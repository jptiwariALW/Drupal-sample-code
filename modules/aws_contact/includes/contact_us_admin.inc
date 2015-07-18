<?php
/**
 * @file
 * Manage meeting functionality in admin.
 */
/**
 * Function to list all the orders
 */
 function enquiry_list() {
    $rows = array();
     $header = array(
      array('data' => 'S.N', 'field' => 'id', 'sort' => 'asc'),
      array('data' => 'User Name', 'field' => 'user_name'),
      array('data' => 'User Email', 'field' => 'user_email'),
      array('data' => 'Question', 'field' => 'question'),
     );
    $results = db_select('aws_travel_question', 'iq')->extend('PagerDefault')->extend('TableSort')
      ->limit(10)
      ->orderByHeader($header)
      ->fields ('iq')
      ->execute();
       if (count($results)) {
         $id = 1;
         foreach ($results as $result) {
          $rows[$result->id]['id'] = $id;
          $rows[$result->id]['user_name'] = $result->user_name;
          $rows[$result->id]['user_email'] = $result->email;
          $rows[$result->id]['question'] = $result->Question;
          $id++;
         }
        
       }
    $build['enquiry_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No enquiry available.'),
  );
  $build['enquiry_pager'] = array('#theme' => 'pager');
  return $build;
}

