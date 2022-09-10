<?php
/**************************************************************************
 *
 * @file user_register.inc
 *
 *************************************************************************/

/*************************************************************************
 * Hook form alter
 ************************************************************************/
function cio_form_user_register_form_alter(&$form, &$form_state, $form_id)
{
	$form['#submit'][] = 'cio_user_register_submit';
}


/*************************************************************************
 * Hook node validate
 ************************************************************************
function cio_user_register_validate($form, &$form_state)
{

}


/*************************************************************************
 * Hook register submit
 ************************************************************************/
function cio_user_register_submit($form, &$form_state)
{	
	$overrides = variable_get('cio_email_overrides', array());
	$skip = isset($overrides['register_no_approval_required']) ? $overrides['register_no_approval_required'] : null;

	if($skip && variable_get('cio_unpublished_stop_send', FALSE))
	{
		unset($_SESSION['messages']);
	}
}


/*************************************************************************
 * Hook node callback
 ************************************************************************
function core_user_register_callback($form, $form_state)
{
	
}*/