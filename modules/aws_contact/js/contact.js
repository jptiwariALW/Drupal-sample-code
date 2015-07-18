/**
 * @file
 * Js to hide/show product's description
 */
var Drupal;
var jQuery;
(function ($) {
    "use strict";
     Drupal.behaviors.product_details = {
        attach: function () {
			 $('#edit-question').after('<div id="qus_error" class="custom_error"><div>');
            $('#edit-acc-email').after('<div id="acc_mail_error" class="custom_error"><div>');
            $('#edit-fname').after('<div id="fname_error" class="custom_error"><div>');
			$('#edit-lname').after('<div id="lname_error" class="custom_error"><div>');
			$('#edit-title').after('<div id="title_error" class="custom_error"><div>');
			$('#edit-email').after('<div id="email_error" class="custom_error"><div>');
			$('#edit-password').after('<div id="pass_error" class="custom_error"><div>');
			$('label[for="edit-term-of-use"]').after('<div id="terms_error" class="custom_error"><div>');
			$('#edit-phone-work').after('<div id="edit-phone-home_error" class="custom_error"><div>');
             $('#edit-submit1').click(function (e) {
                if ($('#edit-question').val() == '') {
                   $('#qus_error').html('Please enter question.');
                    e.preventDefault();
                } else {
                    $('#qus_error').html('');
                }
                
                if ($('#edit-acc-email').val() == '') {
                   $('#acc_mail_error').html('Please enter email.');
                    e.preventDefault();
                } else {
                    $('#acc_mail_error').html('');
                }
                
                if ($('#edit-fname').val() == '') {
                   $('#fname_error').html('Please enter first name.');
                    e.preventDefault();
                } else {
                    $('#fname_error').html('');
                }
				if ($('#edit-lname').val() == '') {
                   $('#lname_error').html('Please enter last name.');
                    e.preventDefault();
                } else {
                    $('#lname_error').html('');
                }
				if ($('#edit-title').val() == '') {
                   $('#title_error').html('Please select title.');
                    e.preventDefault();
                } else {
                    $('#title_error').html('');
                }
            })
        }
    }
}(jQuery));