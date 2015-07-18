Drupal AWS Order Management Module:
------------------------

Overview:
--------
This module is developed for Order management system. Admin can create order and mailed checkout
link to customer for payment. Also view, Sort, Search and export orders.
paypal integrated for payment.

Features:
---------

The AWS Order Management Module:

* Export order's to csv file.
* Search oredrs by using different fields.
* Sort oredrs.
* check payment status by siteadmin.
* E-Mail feature to customer for their order with checkout link.
* Provide easy to call block with name "'Display Order Details" for chekout details and form.


Requriment:
------------
* For Date  pop-up on order form.
date module.

You can download it from:
https://www.drupal.org/project/date

Installation:
------------
1. Download and unpack the Libraries module directory in your modules folder
   (this will usually be "sites/all/modules/").
2. Go to "Administer" -> "Modules" and enable the  AWS Order Management Module.
3. Go to "Administer" -> "Blocks"  and call the block on requried page.

Configuration:
------------
  define('EMAIL_ADD', 'Notification email for paypal');
  define('PAYPAL_EMAIL_ADD', 'Paypal facilitator account email address');
  
* Change the values of defined constant's  by editing module file.

* Provide a Taxonomy term for Currency (salutation). with machine name "currency".
  Add terms as per the requriment.

Author
------
ALW

