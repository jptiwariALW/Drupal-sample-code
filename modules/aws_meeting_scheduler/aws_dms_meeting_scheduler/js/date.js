/**
 * @file
 * Js to add start date and end date validation in manage meeting section
 */
var st_date;
var en_date;
var comp_date;
var Drupal;
var jQuery;
var start_id;
var end_id;
var start_id_arr;
var end_id_arr;
(function ($) {
/**
 * Change the book a meeting form with the conference name
 */
    "use strict";
    Drupal.behaviors.changeFieldsetValue = {
        attach: function () {
            $('form[id=admin-meeting-form]').submit(function () {
                if ($('#edit-meeting-start-date-datepicker-popup-0').val() !== '' && $('#edit-meeting-end-date-datepicker-popup-0').val() !== '') {
                    start_id = $('#edit-meeting-start-date-datepicker-popup-0').val();
                    end_id = $('#edit-meeting-end-date-datepicker-popup-0').val();
                    start_id_arr = start_id.split('-');
                    end_id_arr = end_id.split('-');
                    st_date = new Date(start_id_arr[2] + ',' + start_id_arr[1] + ',' + start_id_arr[0]);
                    en_date = new Date(end_id_arr[2] + ',' + end_id_arr[1] + ',' + end_id_arr[0]);
                    comp_date = en_date - st_date;
                    if (comp_date < 0) {
                        $('#meeting_time_admin').html('End date should be greater than start date');
                        return false;
                    }
                    $('#meeting_time_admin').html('');
                    return true;
                }
                $('#meeting_time_admin').html('');
            });
        }
    };
}(jQuery));