Drupal AWS Contact Module:
------------------------

Overview:
--------
This module is developed for generating contact us form and enquiry form.
This module is also displaying contacts listing in admin section.

Features:
---------

The AWS Contact module:

* Provide block named "Contact form" to call contact form.
* Ajax based model window for enquiry form.
* E-Mail feature to site admin for new enquiry.

Requriment:
------------
* For model window.
ctools module (CHAOS TOOL SUITE).

You can download it from:
https://www.drupal.org/project/ctools

Installation:
------------
1. Download and unpack the Libraries module directory in your modules folder
   (this will usually be "sites/all/modules/").
2. Go to "Administer" -> "Modules" and enable the AWS Contact module.
3. Go to "Administer" -> "Blocks"  and call the block on requried page.

Configuration:
------------
* Provide a Taxonomy term for Title (salutation). with machine name "title".
  Add terms as per the requriment.
  
How to use model box:
------------
* Make sure to have enable ctools module.
* Use  enquiry/%ctools_js  link with anchore tag.
  
Author
------
ALW

