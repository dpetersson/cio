<?php
/**************************************************************************
 *
 * @file cio.admin.inc
 *
 *************************************************************************/

/**************************************************************************
 * Hook settings form
 *************************************************************************/
function cio_config_form($form, &$form_state) 
{
	$form = array();
	
	$form['description'] = array(
		'#markup' => '<p></p>',
	);

	// TODO: check to make sure the API is enabled before trying to build this form
	
	$form['email'] = array(
		'#type' => 'fieldset', 
		'#title' => t('Email Overrides'),
		'#description' => t("Configure email overrides."),
		'#weight' => -3, 
		'#collapsible' => FALSE, 
		'#collapsed' => FALSE,
	);

	$overrides = variable_get('cio_email_overrides', array());
	
	foreach(cio_system_mail_keys() as $key => $title)
	{
		$form['email']['cio_'.$key] = array(
			'#type' => 'select',
			'#options' => cio_email_options(),
			'#title' => $title,
			'#default_value' => isset($overrides[$key]) ? $overrides[$key] : '_none',
			'#description' => t(""),
			'#required' => FALSE,
		);
	}
	/*
	$form['email']['cio_unpublished_stop_send'] = array(
		'#type' => 'checkbox',
		'#title' => t("Disable Drupal from sending the email if the Customer.io email is overridden but inactive."),
		'#default_value' => variable_get('cio_unpublished_stop_send', FALSE),
		'#description' => t(""),
	);
	*/
	
	
	$form['names'] = array(
		'#type' => 'fieldset', 
		'#title' => t('User Account Name Settings'), 
		'#weight' => -2,
		'#description' => t("Define the fields where user first & last names are stored to pass the information to Customer.io."),
		'#collapsible' => FALSE, 
		'#collapsed' => FALSE,
	);
	$form['names']['cio_first_name_field'] = array(
		'#type' => 'select',
		'#options' => cio_account_field_options(),
		'#title' => t("User First Name"),
		'#default_value' => variable_get('cio_first_name_field', ''),
		'#description' => t(""),
		'#required' => FALSE,
	);
	$form['names']['cio_last_name_field'] = array(
		'#type' => 'select',
		'#options' => cio_account_field_options(),
		'#title' => t("User Last Name"),
		'#default_value' => variable_get('cio_last_name_field', ''),
		'#description' => t(""),
		'#required' => FALSE,
	);
	$form['names']['sync_users'] = array(
		'#markup' => '<ul class="action-links"><li><a href="/cio/sync">Sync Existing Users</a></li></ul>',
	);
	
	$form['tracking'] = array(
		'#type' => 'fieldset', 
		'#title' => t('Tracking'), 
		'#weight' => -2,
		'#description' => t("To use in-app messaging, tracking authenticated users must be enabled. Tracking also creates pageview events and allows for the use of !link.", array('!link' => l("Customer.io's JavaScript event tracking", 'https://customer.io/docs/javascript-quick-start/', array('attributes' => array('target' => '_blank'))))),
		'#collapsible' => FALSE, 
		'#collapsed' => FALSE,
	);
	$form['tracking']['cio_track_users'] = array(
		'#type' => 'checkbox',
		'#title' => t("Track authenticated users"),
		'#default_value' => variable_get('cio_track_users', FALSE),
		'#description' => t(""),
	);
	$form['tracking']['cio_track_visitors'] = array(
		'#type' => 'checkbox',
		'#title' => t("Track anonymous visitors"),
		'#default_value' => variable_get('cio_track_visitors', FALSE),
		'#description' => t(""),
	);
	$form['tracking']['cio_track_eu'] = array(
		'#type' => 'checkbox',
		'#title' => t("Use EU tracking version"),
		'#default_value' => variable_get('cio_track_eu', FALSE),
		'#description' => t("Customer.io Documentation: If your account is based in the EU, the JavaScript snippet references track-eu.js. If you use the US-based snippet for an EU-region account, we’ll redirect requests accordingly, but your data may be subject to logging in the US."),
	);
	
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save Configuration'),
		'#prefix' => '<div class="form-actions form-wrapper">',
		'#suffix' => '</div>',
	);
        
	//$form['#validate'][] = 'cio_config_form_validate';
	$form['#submit'][] = 'cio_config_form_submit';

	return $form;
}


/**************************************************************************
 * Validation
 *************************************************************************
function cio_config_form_validate($form, &$form_state) 
{
	// 

}


/**************************************************************************
 * 
 *************************************************************************/
function cio_config_form_submit($form, &$form_state)
{
	$overrides = array();
	foreach(cio_system_mail_keys() as $key => $title)
	{
		if(isset($form_state['values']['cio_'.$key]))	
		{
			if(!empty($form_state['values']['cio_'.$key]) && $form_state['values']['cio_'.$key] != '_none')
			{
				$overrides[$key] = $form_state['values']['cio_'.$key];
			}
		}
	}
	
	variable_set('cio_email_overrides', $overrides);
	variable_set('cio_first_name_field', $form_state['values']['cio_first_name_field']);
	variable_set('cio_last_name_field', $form_state['values']['cio_last_name_field']);
	variable_set('cio_track_users', $form_state['values']['cio_track_users']);
	variable_set('cio_track_visitors', $form_state['values']['cio_track_visitors']);
	variable_set('cio_track_eu', $form_state['values']['cio_track_eu']);
	
	drupal_set_message("System Configuration Saved.");
}