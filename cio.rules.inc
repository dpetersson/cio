<?php
/**************************************************************************
 *
 * @file cio.rules.inc
 *
 *************************************************************************/


/**************************************************************************
 * Hook Rules Info
 *************************************************************************/
function cio_rules_action_info()
{
	return array(
		'cio_action_sendmail' => array(
			'label' => t('Send email with Customer.io'),
			'group' => t('CIO'),
			'parameter' => array(
				'to' => array(
					'type' => 'text',
					'label' => t('To'),
					'description' => t('some description here'),
				),
				'email_id' => array(
					'type' => 'decimal',
					'label' => t('Email'),
					'description' => t('The email from Customer.io'),
					'options list' => '_cio_rules_email_id_options',
					//'restriction' => 'input',
					'default value' => '',
				),
			),
			'base' => 'cio_rules_action_sendmail',
			//'access callback' => 'rules_system_integration_access',
		),
	);
}


/**************************************************************************
 * Get a list of Emails that Customer.io has for sending
 *************************************************************************/
function _cio_rules_email_id_options()
{
	$options = array();
	$emails = cio_get('emails'); // TODO : is this going to be the correct function?
	
	foreach($emails->emails as $email)
	{
		$options[$email->id] = $email->name." | ID:".$email->id; 
	}
	
	return $options;
}


/**************************************************************************
 * Send the email with Customer.io
 *************************************************************************/
function cio_rules_action_sendmail($to, $email_id)
{
	drupal_set_message("to: ".$to);
	drupal_set_message("email_id: ".$email_id);
}