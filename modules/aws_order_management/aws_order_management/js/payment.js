/**
 * @file
 * Js to hide/show product's description
 */
var Drupal;
var jQuery;
var oldText;

(function ($) {
    "use strict";
/**
 * Change the book a meeting form with the conference name  user-pass
 */
  Drupal.behaviors.product_details = {
    attach: function () {
    $('.form-item-payment-due-date-date').removeAttr("data-original-title");
    $('.manageorders a').attr('class' , 'overlay-exclude');
    if($("#edit-roles-6").is(":checked")) {
      $('.group-dmc-user').css('display' , 'block');
    } else {
      $('.group-dmc-user').css('display' , 'none');
    }
	
    $('#edit-submit').click(function(){
		
      var date_value = $('#edit-payment-due-date-datepicker-popup-0').val();		
      if(date_value == '') {
        setTimeout(function() {
          $("label[for='edit-payment-due-date-datepicker-popup-0']").each(function () {			
            $(this).text("Please enter payment due date");
          });
        }, 200);
      }		
    });
	
	$('#edit-reset').click(function(){
        var date_value = $('#edit-order-search').val();
        $(".error").text('');
		$('#edit-order-search').val('');
		if(date_value!='')
		window.location.href="payment";
		
	});
	
    $("#edit-roles-6").click(function() {
      if($(this).is(":checked")) {
        $('.group-dmc-user').css('display' , 'block');
      } else {
        $('.group-dmc-user').css('display' , 'none');
      }
    })
    $('#edit-descrpition-format').css('display' , 'none');
    $('#edit-extra-notes-format').css('display' , 'none');
    $('#edit-currency').attr("disabled", 'disabled');
    $('.vieworders a').addClass('overlay-exclude');
    if ($(".page-user-refund-create").is(":visible")) {
      $('.header.container-fluid').after("<div class='txt_wrap'><div class='view_order_list'>Create a Refund</div></div>" );
    }
            if ($(".page-user-order-create").is(":visible")) {
                $("title").html("Create Order | AWS DMS");
                $('.header.container-fluid').after("<div class='txt_wrap'><div class='view_order_list'>Create Order</div></div>" );
             }
              if ($(".page-admin-payment").is(":visible")) {
                $("title").html("View Orders | AWS DMS");
                $('.header.container-fluid').after("<div class='txt_wrap'><div class='view_order_list'>View Orders</div></div>" );
                //$('.table').Wrap($('<div>').addClass('table-wrap'));
              }
               if ($(".page-admin-order-view").is(":visible")) {
                $('.header.container-fluid').after("<div class='txt_wrap'><div class='view_order_list'>Order Details</div></div>" );
               }
             //$('.sticky-header').css('position', '');
             if ($(".thanx_msg").is(":visible")) {
                $('.vieworders a').attr('class' ,'overlay-exclude');
            }
            $('input[id=edit-price]').after('<div id="price_error" class="custom_error"><div>')   
            $('form[id=admin-order-form]').submit(function (e) {
              if ($('#edit-price').val() !== '') {
	        if (!(/^(?:\d+|\d{1,3}(?:\d{3})+)(?:\.\d+)?$/.test($('#edit-price').val()))) {
		  $('#price_error').html('Please enter a valid price');
          if ($('#edit-fname').val() !== '' && $('#edit-surname').val() !== '' && $('#edit-email').val() !== '' && $('#edit-product-name').val() !== '' && $('#edit-payment-due-date-datepicker-popup-0').val() !== '' ){
            $('html, body').animate({
                  scrollTop: ($('#edit-price').offset().top-50)
                  }, 500);
          }
		  e.preventDefault();
		  return false;
                } else {
		  $('#price_error').html('');
		}
              }
	        else {
		$('#price_error').html('');
	      }
                
            });
            /**************checkout page check box validation*****************************/
          $('label[for=edit-term-of-use]').after('<div id="condition_error" class="custom_error"><div>');
          $('form[id=order-checkout-form]').submit(function (e) {
			 $('input[type=checkbox]').each(function () {
				   	if ($("input:checked").length == 0) {						
						$('#condition_error').html('Please agree to terms and conditions');
						 e.preventDefault();
                         return false;
					} else { 
						$('#condition_error').html('');
						return true;
					}
				});
			   
			});
          /********end of code********admin-refund-form***/
          
          /*************Code to hide editor tool bar on cretae order form**********/
         if ($("#admin-order-form").is(":visible")) {
            CKEDITOR.replace( 'edit-descrpition-value',
	        {
		      toolbar :
	     	  [
		        { name: 'basicstyles', items : [ 'Bold','Italic','Underline' ] },
		        { name: 'paragraph', items : [ 'NumberedList','BulletedList'] },
		      ]
	        });
           CKEDITOR.replace( 'edit-extra-notes-value',
	        {
		        toolbar :
		        [
		               { name: 'basicstyles', items : [ 'Bold','Italic','Underline' ] },
		               { name: 'paragraph', items : [ 'NumberedList','BulletedList'] },
		        ]
	        });
         }
         
          if ($("#admin-refund-form ").is(":visible")) {
            $('#edit-description-format').css('display' , 'none');
            CKEDITOR.replace( 'edit-description-value',
	        {
		      toolbar :
	     	  [
		        { name: 'basicstyles', items : [ 'Bold','Italic','Underline' ] },
		        { name: 'paragraph', items : [ 'NumberedList','BulletedList'] },
		      ]
          });
        }
  
        /*************end of editor code* ****************************/
          
          
     }
  }
}(jQuery));