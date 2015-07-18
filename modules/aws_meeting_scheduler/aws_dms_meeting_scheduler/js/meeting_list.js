/**
 * @file
 * Js of contact us and book a meeting form
 */
var date;
var Drupal;
var jQuery;
var id;
var arr;
var st_date;
var en_date;
var comp_date;
var Pikaday;
var m_st_date;
var m_en_date;
var meeting_start_time;
var meeting_end_time;
var flag;
var picker;
var date;
var meeting_st_date;
var start_date;
/**
 * Function to validate email address
 */
function is_valid_email(email) {
    "use strict";
    var expr = /^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$/;
    return expr.test(email);
}

(function ($) {
/**
 * Change the book a meeting form with the conference name
 */
    "use strict";
    Drupal.behaviors.awsdms_meeting_scheduler = {
        attach: function () {
            $('#country-member').find('h2:first').addClass('disabled');
            $('#country-member').after('<div class="country-member-description" id = "country-member-description"></div>');
            $('#start_date').val($('#meeting_1').siblings('.right').children('.con_start_date').html());
            $('#address1').html($('#meeting_1').children(".con_address").html());
            $('#edit-meeting-fieldset .fieldset-legend').html($('#meeting_1').children(".con_name").html());
            $('#conference_date').html($('#meeting_1').siblings('.right').children(".conference_date").html());
            $('#end_date').val($('#meeting_1').siblings('.right').children('.con_end_date').html());
            $('#meeting_time_zone').val($('#meeting_1').siblings('.right').children(".con_time_zone").html());
            id = $('#meeting_1').attr("name");
            if (id) {
                arr = id.split('_');
                $('#meeting_id').val(arr[1]);
            }
            $('.use-ajaxs').click(function () {
                id = $(this).parent().prev().attr("name");
                arr = id.split('_');
                $('#edit-meeting-fieldset .fieldset-legend').html($(this).parent().siblings().find(".con_name").html());
                $('#edit-meeting-fieldset #address1').html($(this).parent().siblings().find(".con_address").html());
                $('#edit-meeting-fieldset #address2').html($(this).parent().siblings().find(".con_city").html());
                $('#conference_date').html($(this).siblings(".conference_date").html());
                $('#meeting_time_zone').val($(this).siblings(".con_time_zone").html());
                $('#start_date').val($(this).siblings(".con_start_date").html());
                $('#end_date').val($(this).siblings(".con_end_date").html());
                $('#meeting_id').val(arr[1]);
                st_date = new Date($('#meeting_time_zone').val());
                meeting_st_date = new Date($('#start_date').val());
                comp_date = meeting_st_date - st_date;
                if (comp_date < 0) {
                    start_date = st_date;
                } else {
                    start_date = meeting_st_date;
                }
                picker.setMinDate(start_date);
                picker.setMaxDate(new Date($('#end_date').val()));
                picker.gotoDate(new Date($('#start_date').val()));
                date = new Date(start_date);
                picker.setDate(date);
            });
            flag = 0;
            $('form[id=request-meeting-form]').submit(function () {
                if ((!is_valid_email($('#edit-email').val())) && ($('#edit-email').val() !== '')) {
                    $('#meeting_email').html('Please enter a valid email address');
                    flag = 1;
                } else {
                    $('#meeting_email').html('');
                }
                if ($('#datepicker').val() !== '' && $('#edit-time').val() !== '') {
                    st_date = new Date($('#meeting_time_zone').val());
                    en_date = new Date($('#datepicker').val() + ' ' + $('#edit-time').val());
                    m_st_date = new Date($('#start_date').val());
                    m_en_date = new Date($('#end_date').val() + ' ' + '23:59:59');
                    meeting_start_time = en_date - m_st_date;
                    meeting_end_time = m_en_date - en_date;
                    comp_date = en_date - st_date;
                    if (comp_date < 0) {
                        $('#meeting_time').html('This is not a conference date');
                        flag = 1;
                    } else if ((meeting_start_time < 0) || (meeting_end_time < 0)) {
                        $('#meeting_time').html('This is not a conference date');
                        flag = 1;
                    } else {
                        $('#meeting_time').html('');
                        flag = 0;
                    }
                } else {
                    $('#meeting_time').html('');
                    flag = 0;
                }
                if ($('#edit-name').val() !== '') {
                    //^[\\p{L} .'-]+$  ^[A-Za-z][A-Za-z0-9 ]+$
                    if (!(/^[ '-]*[A-Za-z][A-Za-z0-9 '-]+$/.test($('#edit-name').val()))) {
                        $('#conf_name_error').html('Please enter a valid name1');
                        flag = 1;
                    } else {
                        $('#conf_name_error').html('');
                        flag = 0;
                    }
                } else {
                    $('#conf_name_error').html('');
                    flag = 0;
                }
                if (flag === 1) {
                    return false;
                }
            });
            return false;
        }
    };
    $('#edit-time').ptTimeSelect({
        'onClose': function ($self) {
            $self.trigger('change');
        }
    });
    $('#meeting_time_zone').val($('#meeting_1').siblings('.right').children(".con_time_zone").html());
    $('#start_date').val($('#meeting_1').siblings('.right').children('.con_start_date').html());
    $('#end_date').val($('#meeting_1').siblings('.right').children('.con_end_date').html());
    st_date = new Date($('#meeting_time_zone').val());
    meeting_st_date = new Date($('#start_date').val());
    comp_date = meeting_st_date - st_date;
    if (comp_date < 0) {
        start_date = st_date;
    } else {
        start_date = meeting_st_date;
    }
    picker = new Pikaday({
        field: document.getElementById('datepicker')
    });
    picker.setMinDate(new Date(start_date));
    picker.setMaxDate(new Date($('#end_date').val()));
    picker.gotoDate(new Date($('#start_date').val()));
    date = new Date(start_date);
    picker.setDate(date);
}(jQuery));