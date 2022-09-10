<?php
/**************************************************************************
 *
 * @file user_pass.inc
 *
 *************************************************************************/

/*************************************************************************
 * Hook form alter
 ************************************************************************/
function cio_form_user_pass_alter(&$form, &$form_state, $form_id)
{
	global $user;

	if($user->uid > 0)
	{
		$mail = $user->mail;
		$user = drupal_anonymous_user();
		drupal_goto('user/password', array('query' => array('name' => $mail)));
	}


	$form['#submit'][] = 'core_user_pass_submit';
	drupal_set_title(t("Reset your password"));
}


/*************************************************************************
 * Hook Form Submit - User Password
 ************************************************************************/
function cio_user_pass_submit($form, &$form_state)
{
	drupal_set_message(t("Your password reset link has been requested, please check your email."));
}