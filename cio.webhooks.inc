<?php
/**************************************************************************
 *
 * @file cio.webhooks.inc
 *
 *************************************************************************/

/**************************************************************************
 * 
 *************************************************************************/
function cio_webhooks() 
{
	$data = json_decode(file_get_contents('php://input'));

	if(is_object($data))
	{
		// Email Sent Webhook
		if(!empty($data->email))
		{
			if($data->key == variable_get('cio_public_key', '0')) // TODO : verify that this exists for Customer.io
			{
				module_invoke_all('cio_webhook', 'email_sent', $data->email); // TODO : is this still valid?
				watchdog('cio', "Incoming webhook email_sent: !data", array('!data' => json_encode($data->email)));
			}
			else
			{
				watchdog('cio', "Incoming webhook received but key incorrect", array(), WATCHDOG_ERROR);
			}
		}
	}

	// TODO - Error checking and appropriate response

	//'400 Bad Request';
	//'500 Internal Server Error';
	header_remove();
    http_response_code('200');
    header("Cache-Control: no-transform,public,max-age=300,s-maxage=900");
    header('Content-Type: application/json');
    header('Status: 200 OK');
    exit();
}