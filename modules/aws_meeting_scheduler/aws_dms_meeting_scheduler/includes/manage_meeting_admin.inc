<?php
/**
 * @file
 * Manage meeting functionality in admin.
 */
/**
 * Function to list all the registered meeting
 */
 function meeting_list() {
  $rows = array();
  $header = array('S.N', 'Name', 'Conference Schedule On', 'Country', 'Action');
  $button = '<a href="meeting_scheduler/add">' . t('Add meeting') . '</a>';
  $results = db_select('meeting_scheduler', 'ms')->extend('PagerDefault')->extend('TableSort')
      ->limit(10)//10 results per page
      ->orderByHeader($header)
      ->fields ('ms')
      ->execute();
  if (count($results)) {
    $id = 1;
    foreach ($results as $result) {
      $requests = db_select('meeting_request', 'mr')
        ->condition('mr.mid', $result->mid);
      // Add the COUNT expression.
      $requests->addExpression('COUNT(mr.rid)', 'request_count');
      $request_count = $requests->execute()->fetchAssoc();
      $rows[$result->mid]['meeting_id'] = $id;
      $rows[$result->mid]['name'] = $result->name;
      $rows[$result->mid]['created'] = format_date($result->meeting_date, 'custom', 'd-M-Y') . "-" . format_date($result->meeting_end_date, 'custom', 'd-M-Y');
      $rows[$result->mid]['country'] = $result->country;
      $rows[$result->mid]['edit/delete'] = "<a href='meeting_scheduler/{$result->mid}/edit'>" . t('Edit') . "</a> | <a href='meeting_scheduler/{$result->mid}/delete'>" . t('Delete') . "</a> | <a href='meeting_scheduler/{$result->mid}/meeting_request'>" . t('View Request') . " (" . $request_count['request_count'] . ")</a>" ;
      $id++;
    }
  }
  $build['meeting_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No meetings available.'),
    '#prefix' => $button,
  );
  $build['meeting_pager'] = array('#theme' => 'pager');
  return $build;
}

function admin_meeting_form($form, &$form_state, $id = 0) {
  drupal_add_js(drupal_get_path('module', 'aws_dms_meeting_scheduler') . '/js/date.js');
  if ($id) {
    $query = db_select('meeting_scheduler', 'ms');
    $query->fields('ms')
      ->condition('mid', $id);;
    $result = $query->execute()->fetchObject();
  }
  include_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = get_countries_terms();
  $form['meeting_fieldset']['mid'] = array(
    '#title' => t('id:-'),
    '#type' => 'hidden',
    '#default_value' => isset($result->mid) ? $result->mid : '',
  );
  $form['meeting_fieldset']['name'] = array(
    '#title' => t('Name:-'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($result->name) ? $result->name : '',
    '#maxlength' => 50,
  );
  $form['meeting_fieldset']['address1'] = array(
    '#title' => t('Address1:-'),
    '#type' => 'textfield',
    '#default_value' => isset($result->address1) ? $result->address1 : '',
  );
  $form['meeting_fieldset']['address2'] = array(
    '#title' => t('Address2:-'),
    '#type' => 'textfield',
    '#default_value' => isset($result->address2) ? $result->address2 : '',
  );
  $form['meeting_fieldset']['city'] = array(
    '#title' => t('City:-'),
    '#type'  => 'textfield',
    '#default_value' => isset($result->city) ? $result->city : '',
  );  
  $form['meeting_fieldset']['state'] = array(
    '#title' => t('State:-'),
    '#type' => 'textfield',
    '#default_value' => isset($result->state) ? $result->state : '',
  );
  $form['meeting_fieldset']['country'] = array(
    '#type' => 'select',
    '#empty_value' => '',
    '#title' => t('Country:-'),
    '#default_value' => isset($result->country) ? $result->country : '',
    '#options' => $countries,
  );
  $form['meeting_fieldset']['zip_code'] = array(
    '#title' => t('Zip Code:-'),
    '#type' => 'textfield',
    '#default_value' => isset($result->zip_code) ? $result->zip_code : '',
  );
  $form['meeting_fieldset']['meeting_start_date'] = array(
    '#type' => 'date_popup',
    '#required' => TRUE,
    '#title' => t('Conference Start Date:'),
    '#date_label_position' => '',
    '#description' => '',
    '#date_format' => 'd-m-Y',
    '#date_year_range' => '-0:+75',
    '#default_value' => isset($result->meeting_date)?date("Y-m-d H:i:s", $result->meeting_date):'',
    '#attributes' => array('autocomplete' => 'off', 'readonly' => 'readonly'),
    '#datepicker_options' => array('minDate' => 0),
  );
  $form['meeting_fieldset']['meeting_end_date'] = array(
    '#type' => 'date_popup',
    '#required' => TRUE,
    '#date_label_position' => '',
    '#description' => '',
    '#date_format' => 'd-m-Y',
    '#title' => t('Conference End Date:'),
    '#date_year_range' => '-0:+75',
    '#default_value' => isset($result->meeting_end_date)?date("Y-m-d H:i:s", $result->meeting_end_date):'',
    '#attributes' => array('autocomplete' => 'off', 'readonly' => 'readonly'),
    '#datepicker_options' => array('minDate' => 0),
  );
  $form['meeting_fieldset'] ['meeting_time_markup'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="meeting_time_admin">',
    '#suffix' => '</div>',
  );
  $form['meeting_fieldset']['meeting_timezone'] = array(
    '#type' => 'select',
    '#title' => t('Time zone'),
    '#default_value' => isset($result->meeting_timezone) ? $result->meeting_timezone : variable_get('date_default_timezone', ''),
    '#options' => system_time_zones(),
    '#description' => t('Select the desired local time and time zone. Dates and times throughout this site will be displayed using this time zone.'),
  );
  $form['meeting_fieldset']['description'] = array(
    '#title' => t('Meeting Description:-'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#default_value' => isset($result->description) ? $result->description : '',
  );    
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
    '#submit' => array('admin_meeting_form_submit')
  );
  return $form;
}

/**
 * Get countries list from country (3) taxonomy
 */
function get_countries_terms() {
  $terms = taxonomy_get_tree(3);
  $terms_select = array();
  foreach ($terms as $term) {
    $terms_select[$term->name] = $term->name;
  }
  return $terms_select;
}

/**
 * Function to get meeting detail
 */
function get_meeting_detail($mid) {
  return db_select('meeting_scheduler', 'mr')
      ->fields('mr')
      ->condition('mid', $mid, '=')
      ->execute()
      ->fetchAssoc();
}

/**
 * Submit handler for meeting_registration_form_submit().
 * This function is used for insert/update value in table of meeting_registration
 */
function admin_meeting_form_submit($form, &$form_state) {
  try {
    if (isset($form_state['values']['mid']) && $form_state['values']['mid'] != '') {
      $mid = $form_state['values']['mid'];
      $query = db_update('meeting_scheduler');
    }
    else {
      $query = db_insert('meeting_scheduler');
    }
    $query->fields(array(
      'name' => $form_state['values']['name'],
      'address1' => $form_state['values']['address1'],
      'address2' => $form_state['values']['address2'],
      'country' => $form_state['values']['country'],
      'state' => $form_state['values']['state'],
      'city' => $form_state['values']['city'],
      'zip_code' => $form_state['values']['zip_code'],
      'meeting_date' => strtotime($form_state['values']['meeting_start_date']),
      'meeting_end_date' => strtotime($form_state['values']['meeting_end_date'] . ' 23:59:59'),
      'meeting_timezone' => $form_state['values']['meeting_timezone'],
      'description' => $form_state['values']['description'],
      'created' => time(),
    ));
    if (isset($mid)) {
      $query->condition('mid', $mid);
    }      
    $query->execute();
    if (isset($mid)) {
      drupal_set_message(t('Meeting has been updated successfully.'));
    }
    else {
      drupal_set_message(t('Meeting has been scheduled successfully.'));
      drupal_goto('admin/meeting_scheduler'); 
    }  
  } catch (PDOException $e) {
    drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
  }     
}

/**
 * admin_deletemeeting_form().
 * This function is used for confirm delete message for category.
 */
function admin_deletemeeting_form($form, &$form_state, $mid) {
  $form['mid'] = array(
    '#type' => 'value',
    '#value' => $mid,
  );
  return confirm_form($form,
       t('Are you sure you want to delete this meeting?'),
       isset($_GET['destination']) ? $_GET['destination'] : "admin/meeting_scheduler",
       t(''),
       t('Delete'),
       t('Cancel'));
}

/**
 * Submit handler for admin_deletemeeting_form_submit().
 * This function is used for delete meeting.
 */
function admin_deletemeeting_form_submit($form, &$form_state) {
  $meeting_id = $form_state['values']['mid'];
  $num_deleted = db_delete('meeting_scheduler')
      ->condition('mid', $meeting_id)
      ->execute();
  drupal_set_message(t('Meeting has been deleted successfully.')); 
  drupal_goto('admin/meeting_scheduler'); 
}

/**
 * Request list when customer request for a particular meeting
 */
function meeting_request_list($mid) {
  $rows = array();
  $url = url('admin/meeting_scheduler');
  $button = '<a href="' . $url . '">' . t('>>Back') . '</a>';
  $detail = get_meeting_detail($mid);
  $button .= '<h2>' . $detail['name'] . ' Schedule in ' . $detail['country'] . '</h2>';
  $header = array('S.N', 'Name', 'Email', 'Meeting Date', 'Action');
  $results = db_select('meeting_request', 'mr')
      ->condition('mid', $mid, '=')->extend('PagerDefault')->extend('TableSort')
      ->limit(10)//10 results per page
      ->orderByHeader($header)
      ->fields ('mr')
      ->execute();
  if (count($results)) {
    $id = 1;
    foreach ($results as $result) {
      $rows[$result->rid]['request_id'] = $id;
      $rows[$result->rid]['name'] = $result->name;
      $rows[$result->rid]['email'] = $result->email;
      $rows[$result->rid]['created'] = format_date($result->booking_date, 'custom', 'Y-m-d H:i:s');
      $rows[$result->rid]['view'] = "<a href='" . $url . '/' . $result->rid . "/view_request'>" . t('View') . "</a>" ;
      $id++;
    }
  }
  $build['meeting_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#prefix' => $button,
    '#empty' => t('No request available.'),
  );
  $build['meeting_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Function to view meeting request
 */
function meeting_request_view($form, &$form_state, $rid) {
  $result = db_select('meeting_request', 'mr')
      ->fields('mr')
      ->condition('rid', $rid, '=')
      ->execute()
      ->fetchAssoc();
  $url = url('admin/meeting_scheduler/' . $result['mid'] . '/meeting_request');
  $detail = get_meeting_detail($result['mid']);
  $html = '';
  $html .= '<p>Name: ' . $result['name'] . '</p>'; 
  $html .= '<p>Email: ' . $result['email'] . '</p>'; 
  $html .= '<p>Company: ' . $result['company'] . '</p>'; 
  $html .= '<p>Message: ' . $result['message'] . '</p>';
  if ($result['comment'] != '') {
    $html .= '<p>Your Feedback: ' . $result['comment'] . '</p>'; 
  }    
  $form['box'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="box">',
    '#suffix' => '</div>',
    '#markup' => '<h1>Request for meeting ' . $detail['name'] . ' Schedule in ' . $detail['country'] . '</h1> <a href="' . $url . '">' . t('>>Back') . '</a>' . $html,
  );
  if ($result['status'] == '0') {
    $form['name'] = array(
      '#type' => 'text',
      '#name' => 'name',
      '#default_value' => $result['name'],
    );
    $form['email'] = array(
      '#type' => 'text',
      '#name' => 'email',
      '#default_value' => $result['email'],
    );
    $form['comment'] = array(
      '#type' => 'textarea',
      '#name' => 'comment',
      '#prefix' => '<div id="box">',
      '#suffix' => '</div>',
    );
    $form['rid'] = array(
      '#type' => 'hidden',
      '#default_value' => $rid,
    );
    $form['allow'] = array(
      '#type' => 'submit',
      '#value' => t('Allow'),
    );
    $form['deny'] = array(
     '#type' => 'submit',
      '#value' => t('Deny'),
    );
  }
  return $form;
}

/**
 * Function to approve/deny meeting request
 */
function meeting_request_view_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Allow') {
    $status = 1;
  } 
  else {
    $status = 2;
  }    
  $query = db_update('meeting_request')
      ->fields(array(
        'comment' => $form_state['values']['comment'],
        'request_updated' => time(),
        'status' => $status,
      ))
      ->condition('rid', $form_state['values']['rid'])
      ->execute();
  $message = "Hi " . $form_state['values']['name'] . ", \n\n";
  $message .= $form_state['values']['comment'];
  $subject = "Meeting Status";
  $to = $form_state['values']['meeting_fieldset']['email'];
  $from = variable_get('site_mail', ini_get('sendmail_from'));
  $params['message'] = $message;
  $params['subject'] = $subject; 
  drupal_mail('aws_dms_meeting_scheduler', 'key', $to, language_default(), $params, $from, TRUE);
  if ($status == 1) {
    drupal_set_message(t('Request has been allowed successfully.'));
  }
  else {
    drupal_set_message(t('Request has been denied successfully.'));
  }
}