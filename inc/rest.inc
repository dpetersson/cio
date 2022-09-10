<?php
/**************************************************************************
 *
 * @file rest.inc
 *
 *************************************************************************/


/*************************************************************************
 * Track Customer
 *
 * Example Usage:
 	
 	cio_track_customer($id, array(
		'email' => 'bob@bob.com',
		'created_at' => 1361205308,
		'first_name' => 'Bob',
		'last_name' => 'Bob',
		'some_value' => "I'm here",
	));
 *
 *
 ************************************************************************/
function cio_track_customer($id, $data, $token = null)
{
	$auth = cio_auth('tracking', $token);
	
	return cio_curl('PUT', $auth, 'https://track.customer.io/api/v1/customers/'.$id, $data);
}


/*************************************************************************
 * Track Event
 *
 * Example Usage:
 	
 	cio_track_event($id, array(
		'name' => 'purchase',
		'data' => array(
			'price' => '125.00',
			'product' => 'Application Fee',
		),
		'timestamp' => time(),
	));
 *
 *
 ************************************************************************/
function cio_track_event($id, $data, $token = null)
{
	$auth = cio_auth('tracking', $token);
	
	return cio_curl('POST', $auth, 'https://track.customer.io/api/v1/customers/'.$id.'/events', $data);
}


/*************************************************************************
 * Send Transactional Email
 *
	$data = array(
		'transactional_message_id' => {numeric id},
		'to' => {email address},
		'identifiers' => array(
			'id' => {user identifier},
		),
		'message_data' => {data array for email content},
		'bcc' => {bcc email},
		'disable_message_retention' => {optional setting},
		'send_to_unsubscribed' => {optional setting},
		'tracked' => {optional setting},
		'queue_draft' => {optional setting},
		'disable_css_processing' => {optional setting},
	);
 *
 ************************************************************************/
function cio_send_transactional_email($data, $token = null)
{
	$auth = cio_auth('app', $token);
	
	return cio_curl('POST', $auth, 'https://api.customer.io/v1/send/email', $data);
}


/*************************************************************************
 * Get Transactional Messages
 *
 * Returns a list of messages available in Customer.io
 *
 ************************************************************************/
function cio_get_transactional_messages($token = null)
{
	$auth = cio_auth('app', $token);
	
	return cio_curl('GET', $auth, 'https://beta-api.customer.io/v1/api/transactional');
}



/*************************************************************************
 * CURL
 *
 *  $auth['type'] : app or tracking // authentication is slightly different
 *  $op : ('POST', 'PUT', 'GET')
 * 
 ************************************************************************/
function cio_curl($op, $auth, $url, $data = null)
{
	if($data)
	{
		$data = json_encode($data);
	}
	
	$ch = curl_init($url);
	if($auth['type'] == 'app')
	{
		// App API
		curl_setopt($ch, CURLOPT_HTTPHEADER, array(
		    'Content-Type: application/json',
		    'Authorization: Bearer '.$auth['token'],
		    'Content-Length: '.strlen($data),
		));
	}
	else
	{
		// Tracking API
		curl_setopt($ch, CURLOPT_HTTPHEADER, array(
		    'Content-Type: application/json',
		    'Content-Length: '.strlen($data),
		));
		curl_setopt($ch, CURLOPT_USERPWD, $auth['token']);
	}
 
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $op);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
	$response = curl_exec($ch);
	curl_close($ch);
	
	//watchdog('cio', "There was an error", array(), WATCHDOG_ERROR);
	
	return json_decode($response);
}