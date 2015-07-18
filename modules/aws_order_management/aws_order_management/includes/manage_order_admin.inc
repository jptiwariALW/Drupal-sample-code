<?php
/**
 * @file
 * Manage orders in admin.
 */

/**
 * Function to list all the orders.
 */
function order_list() {
  global $user; 
  $rows = array();
  $url = url('admin/order');
  // Check if there is sorting request.
  if (isset($_REQUEST['sort']) && isset($_REQUEST['order'])) {
    // Sort it Ascending or Descending?.
    if ($_GET['sort'] == 'asc') {
      $sort = 'ASC';
    }
    else {
      $sort = 'DESC';
    }
    // Which column will be sorted.
    switch ($_GET['order']) {
      case 'Order Id':
        $order = 'order_id';
        break;
      case 'Author':
        $order = 'author_dmc_user';
        break;
      case 'Customer First Name':
        $order = 'customer_name';
        break;
      case 'Customer Surname':
        $order = 'customer_surname';
        break;    
      case 'Status':
        $order = 'order_status';
        break;
      case 'Product Name':
        $order = 'product_name';
        break;
      case 'Amount':
        $order = 'price';
        break;
      case 'Payment Due Date':
        $order = 'payment_due_date';
        break;
      case 'Currency':
        $order = 'currency';
        break;        
      case 'Creator Name':
        $order = 'order_creator';
        break;
      default:
        $order = 'order_id';
    }
  }
  else {
    // Default sort.
    $sort = 'DESC';
    $order = 'id';
  }  
  if (isset($_REQUEST['op']) && $_REQUEST['op'] == 'Reset') {
    drupal_goto('admin/payment');
  }
  $current_user_id = $user->uid;
  $query = db_select('payment_order_table', 'ot');
  $header = array(array('data' => t('Order ID'), 'field' => 'order_id'), array('data' => t('Author'), 'field' => 'author_dmc_user'), array('data' => t('Customer First Name'), 'field' => 'customer_name') ,  array('data' => t('Customer Surname'), 'field' => 'customer_surname'), array('data' => 'Product Name', 'field' => 'product_name'), array('data' => 'Amount', 'field' => 'price'), array('data' => 'Currency', 'field' => 'currency'), array('data' => 'Creation Date', 'field' => 'created'), array('data' => 'Payment Due Date', 'field' => 'payment_due_date'), array('data' => 'Creator Name', 'field' => 'order_creator'), array('data' => 'Status', 'field' => 'order_status'));
  $db_or = db_or();
  if (isset($_GET['order_search']) && ($_GET['order_search']!='')) {
    $order_search_n = $_GET['order_search'];
    $db_or->condition('ot.order_id', $order_search_n, '=');
    $db_or->condition('ot.customer_name', '%' . db_like($order_search_n) . '%', 'LIKE');
    $db_or->condition('ot.product_name', '%' . db_like($order_search_n) . '%', 'LIKE');
    $db_or->condition('ot.customer_surname', '%' . db_like($order_search_n) . '%', 'LIKE');
    $db_or->condition('ot.order_creator', '%' . db_like($order_search_n) . '%', 'LIKE');
    $query->condition($db_or);
  } 
  $results = $query->extend('PagerDefault')->extend('TableSort')
    ->limit(10)   
    ->orderBy($order, $sort)
    ->fields ('ot')
    ->execute();
  $order_status_array = array('0' => t('Pending'), '1' => t('Completed'), '2' => 'Expired', '3' => 'Refund');
  if (count($results)) {
    $id = 1;
    foreach ($results as $result) {    
      $rows[$result->id]['order_id'] = "<a href='" . $url . '/' . $result->order_id . "/view'>" . $result->order_id . "</a>";
      if (!in_array($current_user_role, $user->roles)) {
        $rows[$result->id]['author_dmc_user'] = $result->author_dmc_user;
      }
      $rows[$result->id]['customer_name'] =  $result->customer_name;
      $rows[$result->id]['customer_surname'] = $result->customer_surname;
      $rows[$result->id]['product_name'] = $result->product_name;
      if ($result->order_status == '3') { //check for refund order
        $rows[$result->id]['price'] = ($result->refund_amount) ? $result->refund_amount : 'NA';
      } 
      else {
        $rows[$result->id]['price'] = ($result->price) ? $result->price : 'NA';
      }
      $rows[$result->id]['currency'] = $result->currency;
      $rows[$result->id]['created'] = format_date($result->created, 'custom', 'd-M-Y');
      $rows[$result->id]['payment_due_date'] = ($result->payment_due_date) ? format_date($result->payment_due_date, 'custom', 'd-M-Y') : 'NA';
      $rows[$result->id]['order_creator'] = ($result->order_creator) ? $result->order_creator :'NA';
      $rows[$result->id]['order_status'] = $order_status_array[$result->order_status];
      $id++;
    }
  }
  $build['meeting_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No orders available.'),
    '#prefix' => render(drupal_get_form('order_search_form', 'order_search')) . '<div class="orderlist_table_wrapper">',
    '#suffix' => '</div>',
  );
  $build['order_pager'] = array('#theme' => 'pager');
  return $build;
}


function order_search_form($form, &$form_state, $id) {
  drupal_add_js(drupal_get_path('module', 'aws_order_management') . '/js/payment.js');
  global $user;
  $form['#method'] = 'get'; 
  $form['order_search'] = array(
    '#title' => t('Search'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($_GET['order_search'])? $_GET['order_search']:'',
  );
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',    
    '#submit' => array('order_search_form_submit')
  );
  if (isset($_GET['order_search']) && ($_GET['order_search']!='')) {
    $srch = $_GET['order_search'];
    $url = "/admin/payment/export?order_search=$srch";
  }
  else {
    $url ='/admin/payment/export';
  }
  $form['reset'] = array(
    '#type' => 'button',
    '#value' => t('Reset'),
    '#attributes' => array('onclick' => 'this.form.reset(); return false;'),
    '#suffix' =>  '<a class ="verlay-exclude" href="' . $url . '" >' . t('Export Orders') . '</a>',
  );
  return $form;
}

function submission_list_reset($form, &$form_state) {
  $form_state['values']['order_search'] = '';
}

/**
 * Submit handler for meeting_registration_form_submit().
 * This function is used for insert/update value in table of order table
 */
function order_search_form_submit($form, &$form_state) {
  global $user;
}

function admin_order_form($form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'aws_order_management') . '/js/payment.js');
  $form['order_id'] = array(
    '#title' => t('id:-'),
    '#type' => 'hidden',
   );
  $form['order_fieldset']['fname'] = array(
    '#title' => t('Customer first name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['order_fieldset']['surname'] = array(
    '#title' => t('Customer surname'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['order_fieldset']['email'] = array(
    '#title' => t('Customer Email'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['order_fieldset']['product_name'] = array(
    '#title' => t('product name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['order_fieldset']['price'] = array(
    '#title' => t('Amount'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['order_fieldset']['currency'] = array(
    '#title' => t('Currency'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['order_fieldset']['payment_due_date'] = array(
    '#type' => 'date_popup',
    '#required' => TRUE,
    '#title' => t('PAYMENT DUE DATE'),
    '#date_label_position' => '',
    '#description' => '',
    '#date_format' => 'd-m-Y',
    '#date_year_range' => '-0:+75',
    '#attributes' => array('autocomplete' => 'off', 'readonly' => 'readonly'),
    '#datepicker_options' => array('minDate' => 0),
   );
  $form['order_fieldset']['country'] = array(
    '#title' => t('COUNTRY'),
     '#type' => 'hidden',
  );
  $form['order_fieldset']['order_creator'] = array(
    '#title' => t('Order Creator'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['order_fieldset']['descrpition'] = array(
    '#title' => t('DESCRIPTION'),
    '#type' => 'text_format',
  );
  $form['order_fieldset']['extra_notes'] = array(
    '#title' => t('EXTRA NOTES'),
    '#type' => 'text_format',
  );
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
    '#submit' => array('admin_order_form_submit')
  );
  return $form;
}
/**
 * Validate handler for refund form email field.
 */
function admin_refund_form_validate($form, &$form_state) {
  $mail = $form_state['values']['email'];
  if (!valid_email_address($mail)) {
    form_set_error('acc_email', t('Please enter valid email address.'));
  }
}


/**
 * Validate handler for order form email field.
 */
function admin_order_form_validate($form, &$form_state) {
  $mail = $form_state['values']['email'];
  if (!valid_email_address($mail)) {
    form_set_error('acc_email', t('Please enter valid email address.'));
  }
}

/**
 * Submit handler for admin_order_form_submit().
 */
function admin_order_form_submit($form, &$form_state) {
  global $user;
  echo $user->uid;
  if (user_is_logged_in()) {
    $account = user_load($user->uid);
    $country = $form_state['values']['country'];
    $last_id = db_query('SELECT MAX(id) FROM {payment_order_table}')->fetchCol();;
    if ($last_id[0]) {
      $id = $last_id[0]+1;
    }
    else {
      $id = 1000;
    }
      $order_id = strtoupper(substr($country, 0, 2)) . $id;
  }
  try {
      $query = db_insert('payment_order_table');
      $query->fields(array(
        'order_id' => $order_id,
        'customer_name' => trim($form_state['values']['fname']),
        'customer_surname' => trim($form_state['values']['surname']),
        'customer_email' => trim($form_state['values']['email']),
        'price' => $form_state['values']['price'],
        'currency' => $form_state['values']['currency'],
        'product_name' => trim($form_state['values']['product_name']),
        'description' => $form_state['values']['descrpition']['value'],
        'payment_due_date' => strtotime($form_state['values']['payment_due_date'] . ' 23:59:59'),
        'extra_notes' => $form_state['values']['extra_notes']['value'],
        'order_status' => 0,
        'created' => time(),
        'order_creator' => trim($form_state['values']['order_creator'])
      ));
    $result =$query->execute();
    if ($result) {
     /*****Code send Ckeckout url to customer *****/
      global $base_url;
      $pay_url = $base_url . '/checkout/' . base64_encode($order_id) . '/cart';
      $subject = t('Order Checkout');
      $message = '<div style="font-family: georgia; font-size:11px; color:#000;"> You can pay for your order <a href="' . $pay_url . '" style="color:#f16231; text-decoration:none;" target="_blank">here</a></div>';
        $message =' <tr>
        <td align="left" valign="top" style="padding-top:0px; font-family: georgia; font-size:12px; color:#000; line-height:14px">
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td style="font-family: georgia; font-size:15px; color:#000; line-height: 26px;">Hello ' . $form_state['values']['fname'] . ' ' . $form_state['values']['surname'] . ',</td>
        </tr>
        <tr>
          <td style="font-family: georgia; font-size:15px; line-height: 21px; color:#000;">You can pay for your order <a href="' . $pay_url . '" style="color:#f16231; text-decoration:none;" target="_blank">Click here</a></td>
        </tr>
        </table>
        <p style="font-family: georgia; font-size:15px; color:#000; line-height: 16px;">Thanks</p>
        <p style="color:#333; padding:0px; margin-top: -14px;"><a href="' . $base_url . '">' . str_replace("http://", "", $base_url) . '</a></p>
        </td>
        </tr>';
      $to = $form_state['values']['email'];
      $current_user_email = $user->mail;
      $default_from = ($current_user_email) ? $current_user_email : variable_get('site_mail', ini_get('sendmail_from'));//variable_get('site_mail', ini_get('sendmail_from'));
      $message['headers']['From'] = 'AWS';
      $from = $default_from;
      
      $params['message'] = $message;
      $params['subject'] = $subject;
      drupal_mail('aws_order_management', 'key', $to, language_default(), $params, $from, TRUE);
      drupal_set_message(t('Order has been created successfully.'));
      global $user;
      if (user_is_logged_in()) {
        if (in_array('DMC User', $user->roles)) {
          drupal_goto('user/order/create'); 
         }
      } 
      else {
        drupal_goto('admin/payment'); 
      }
    }
  } catch (PDOException $e) {
    drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
  }     
}