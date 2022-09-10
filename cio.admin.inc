<?php
/**************************************************************************
 *
 * @file cio.admin.inc
 *
 *************************************************************************/

/**************************************************************************
 * Hook settings form
 *************************************************************************/
function cio_settings_form($form, &$form_state) 
{
	$form['cio_tracking_site_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Customer.io Tracking Site ID'),
		'#default_value' => variable_get('cio_tracking_site_id', ''),
		'#size' => 60,
		'#description' => t("Your Customer.io Tracking Site ID."),
		'#required' => TRUE,
	);
	
	$form['cio_tracking_api_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Customer.io Tracking API Key'),
		'#default_value' => variable_get('cio_tracking_api_key', ''),
		'#size' => 60,
		'#description' => t("Your Customer.io Tracking API Key."),
		'#required' => TRUE,
	);
	
	$form['cio_app_api_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Customer.io App API Key'),
		'#default_value' => variable_get('cio_app_api_key', ''),
		'#size' => 60,
		'#description' => t("Your Customer.io App API Key."),
		'#required' => TRUE,
	);
	
	$form['cio_tracking_org_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Customer.io Tracking Org ID'),
		'#default_value' => variable_get('cio_tracking_org_id', ''),
		'#size' => 60,
		'#description' => t("Your Customer.io Tracking Org ID. (The Org ID is only necessary for sending In-App messages.)"),
		'#required' => FALSE,
	);
	
	//$form['#validate'][] = 'cio_settings_form_validate';
	//$form['#submit'][] = 'cio_settings_form_callback';

	return system_settings_form($form);
}


/**************************************************************************
 * Validation
 *************************************************************************
function cio_settings_form_validate($form, &$form_state) 
{
	// TODO
	// test the keys to see if they work... 
}


/**************************************************************************
 * Rebuild Submit
 *************************************************************************
function cio_settings_form_callback($form, &$form_state)
{
	
}
*/