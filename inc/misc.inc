<?php
/**************************************************************************
 *
 * @file misc.inc
 *
 *************************************************************************/


/**************************************************************************
 * Track
 *************************************************************************/
function cio_track()
{
	global $user;
	
	if($user->uid > 0 && variable_get('cio_track_users', FALSE) || $user-uid == 0 && variable_get('cio_track_visitors', FALSE))
	{
		$identify = $user->uid > 0 ? "_cio.identify({id: '".$user->uid."'})" : '';
		$version = variable_get('cio_track_eu', FALSE) ? 'https://assets.customer.io/assets/track-eu.js' : 'https://assets.customer.io/assets/track.js';
		$site_id = variable_get('cio_tracking_site_id', '');
		$org_id = variable_get('cio_tracking_org_id', '');
		
		if($site_id != "" && $org_id != "")
		{
			drupal_add_js("var _cio = _cio || [];
				(function() {
				var a,b,c;a=function(f){return function(){_cio.push([f].concat(Array.prototype.slice.call(arguments,0)))}};b=[\"load\",\"identify\",\"sidentify\",\"track\",\"page\"];for(c=0;c<b.length;c++){_cio[b[c]]=a(b[c])};
				var t = document.createElement('script'), s = document.getElementsByTagName('script')[0];
				t.async = true;
				t.id = 'cio-tracker';
				t.setAttribute('data-site-id', '".$site_id."');
				t.setAttribute('data-use-array-params', 'true');
				t.setAttribute('data-in-app-org-id', '".$org_id."');
				t.setAttribute('data-use-in-app', 'true');
				t.src = '".$version."';
				s.parentNode.insertBefore(t, s);
				})();".$identify, array(
					'type' => 'inline',
					'scope' => 'footer',
					'weight' => 0,
				)
			);
		}
	}
}


/**************************************************************************
 * Email Options List
 *************************************************************************/
function cio_email_options()
{
	$emails = cio_get_transactional_messages();
	$options = array('_none' => "- None -");
	foreach($emails->messages as $email)
	{
		if($email->id > 1) // skip 1 for Customer.io
		{
			$options[(int) $email->id] = $email->name." ID:".$email->id.""; 
		}
	}
	
	return $options;
}


/*************************************************************************
 * cio Sendmail - sending system emails
 *
 
 	Common Drupal Email Variables
 	
 	'site-name' => variable_get('site_name', ""),
	'site-url' => $base_url,
	'site-login-url' => $base_url."/user/login",
	'site-url-brief' => $base_url,
	'user-name' => $account->name,
	'user-mail' => $account->mail, 
	'user-edit-url' => $base_url."/user/".$account->uid."/edit",
	'user-one-time-login-url' => user_pass_reset_url($account), 
	'user-cancel-url' => user_cancel_url($account),
	'inviter' => !empty($account->realname) ? $account->realname : $account->mail,
	'invitation-url' => $base_url."/invitation/".$recipient,
	'group' => $group->title,
	
 *
 ************************************************************************/
function cio_sendmail($id, $message)
{
	global $base_url;

	switch($message['key'])
	{
		case 'invite':
			
			$account = $message['params']['user']; // the person doing the inviting
			$account = user_load($account->uid);
			$account->realname = cio_account_data($account, 'first')." ".cio_account_data($account, 'last');
			
			$recipient = $message['to']; // the person being invited
			$group = $message['params']['group']; // the group being invited to
			
			cio_track_event($account->uid, array(
				'name' => 'sent_invitation',
				'data' => array(
					'invitee_email' => $recipient,
					'team' => $group
				),
				'timestamp' => time(),
			));
			
			$send = cio_send_transactional_email(array(
				'transactional_message_id' => $id,
				'to' => $recipient,
				'identifiers' => array(
					'id' => $recipient,
				),
				'message_data' => array(
					'site-name' => variable_get('site_name', ""),
					'site-url' => $base_url,
					'site-login-url' => $base_url."/user/login",
					'site-url-brief' => $base_url,
					'inviter' => !empty($account->realname) ? $account->realname : $account->mail,
					'invitation-url' => $base_url."/user/register?destination=invitations&mail=".$recipient, //assumes in most cases they won't have an account
					'group' => $group->title,
				),
			));
			
		break;
		
		
		case 'register_admin_created': // Welcome, Account Created by Administrator
		case 'register_no_approval_required' : // Welcome to site
		case 'register_pending_approval' : // Welcome, Account is pending approval
		case 'register_pending_approval_admin' : // Admin - account is pending approval
					
			$account = $message['params']['account'];
			$account = user_load($account->uid);
			
			cio_track_customer($account->uid, array(
				'email' => $account->mail,
				'created_at' => time(),
				'first_name' => cio_account_data($account, 'first'),
				'last_name' => cio_account_data($account, 'last'),
			));
			
			$send = cio_send_transactional_email(array(
				'transactional_message_id' => $id,
				'to' => $account->mail,
				'identifiers' => array(
					'id' => $account->uid,
				),
				'message_data' => array(
					'site-url' => $base_url,
					'site-login-url' => $base_url."/user/login",
					'user-one-time-login-url' => user_pass_reset_url($account).'/login?destination=onboarding', 
				),
			));
			
		break;
		
		case 'password_reset' : // Password Reset
		case 'status_activated' : // Accounted Activated		
		case 'status_blocked' : // Account Blocked
		case 'status_canceled' : // Account Closed
				
			$account = $message['params']['account'];
			$account = user_load($account->uid);
			
			$send = cio_send_transactional_email(array(
				'transactional_message_id' => $id,
				'to' => $account->mail,
				'identifiers' => array(
					'id' => $account->uid,
				),
				'message_data' => array(
					'site-url' => $base_url,
					'site-login-url' => $base_url."/user/login",
					'user-one-time-login-url' => user_pass_reset_url($account).'/login', 
				),
			));
			
		break;
		
		case 'user_registrationpassword':
			
			// not sure if this is the right mail key...
			
		break;
		
		case 'cancel_confirm' : // Account Closed, Please Confirm
		
			$account = $message['params']['account'];
			$account = user_load($account->uid);
			
			$send = cio_send_transactional_email(array(
				'transactional_message_id' => $id,
				'to' => $account->mail,
				'identifiers' => array(
					'id' => $account->uid,
				),
				'message_data' => array(
					'site-url' => $base_url,
					'site-login-url' => $base_url."/user/login",
					'user-cancel-url' => user_cancel_url($account),
				),
			));
		
		break;
	}

	if(isset($send->meta->error))
	{
		watchdog('cio', "@name email attempt to send to @recipient was unsuccessful. @error", array('@name' => $message['key'], '@recipient' => $account->mail, '@error' => $send->meta->error), WATCHDOG_ERROR);
		return FALSE;
	}
	else
	{
		watchdog('cio', "@name email successfully sent to @recipient", array('@name' => $message['key'], '@recipient' => $account->mail), WATCHDOG_INFO);
		return TRUE;
	}
}


/**************************************************************************
 * Make a list of options for name fields
 *************************************************************************/
function cio_account_field_options()
{
	$fields = field_info_instances('user');
	$options = array('_none' => "- None -");
	
	foreach($fields['user'] as $key => $field)
	{
		switch($field['widget']['module'])
		{
			case 'name':
			
				foreach(array('given', 'middle', 'family') as $component)
				{
					$options[$key.'__'.$component] = $field['label'].' ('.$component.')';
				}
			
			break;
			
			case 'text':
			
				$options[$key] = $field['label'];
			
			break;
		}
	}
	
	return $options;
}


/**************************************************************************
 * Get account real name if they exist
 *************************************************************************/
function cio_account_data($account, $op)
{
	$var = 'cio_'.$op.'_name_field';
	$field = variable_get($var, '');
	if(!empty($field))
	{
		$parts = explode('__', $field);
		$field = isset($parts[0]) ? $parts[0] : null;
		$component = isset($parts[1]) ? $parts[1] : null;

		if(isset($account->{$field})) 
		{
			if($component)
			{
				if(!empty($account->{$field}['und'][0][$component]))
				{
					return $account->{$field}['und'][0][$component];
				}
			}
			
			if(!empty($account->{$field}['und'][0]['value']))
			{
				return $account->{$field}['und'][0]['value'];
			}
		}
	}
		
	return '';
}


/**************************************************************************
 * Get the auth token by type
 *************************************************************************/
function cio_auth($type, $token = null)
{
	if(!$token)
	{
		if($type == 'app')
		{
			// App API
			$token = variable_get('cio_app_api_key', '');
		}
		else
		{
			// Tracking API
			$token = variable_get('cio_tracking_site_id', '').":".variable_get('cio_tracking_api_key', '');
		}
	}
	
	return array(
		'type' => $type,
		'token' => $token,
	);
}


/*************************************************************************
 * Get All Site Users
 ************************************************************************/
function cio_get_users()
{
	$query = db_select('users', 'u');
	$query->fields('u', array('uid'))
		  ->condition('u.status', 1, '=');
    $result = $query->execute();
	
    while($row = $result->fetchAssoc())
    {
	    
		$data = new stdClass();
		$data->uid = $row['uid'];
		/*
		$data->name = $row['name'];
		$data->mail = $row['mail'];
		$data->created = $row['created'];
		$data->access = $row['access'];
		$data->status = $row['status'];
		*/
		$rows[] = $data;
    }

	return $rows;
}