<?php
/**************************************************************************
 *
 * @file user_pass_reset.inc
 *
 *************************************************************************/

/*************************************************************************
 * Hook form alter
 ************************************************************************
function cio_form_user_pass_reset_alter(&$form, &$form_state, $form_id)
{
	
}


/*************************************************************************
 * Hook Form Submit - User Password Reset
 *
 *  this submit action is triggered by the user_profile_form
 *
 ************************************************************************/
function cio_user_pass_reset_submit($form, &$form_state)
{
	unset($_SESSION['messages']);
	drupal_set_message(t("Your password has been successfully set."));
}