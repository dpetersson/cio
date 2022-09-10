<?php
/**************************************************************************
 *
 * @file cio.sync.inc
 *
 *************************************************************************/

/**************************************************************************
 * 
 *************************************************************************/
function cio_sync() 
{
	$count = 0;
	$temp = 0;
	$users = cio_get_users();
	
	foreach($users as $user)
	{
		$account = user_load($user->uid);
		
		cio_track_customer($account->uid, array(
			'email' => $account->mail,
			'created_at' => $account->created,
			'first_name' => cio_account_data($account, 'first'),
			'last_name' => cio_account_data($account, 'last'),
		));
		
		$temp++;
		$count++;
		
		if($temp > 50)
		{
			sleep(2); // wait 2 seconds between each 50 users
			$temp = 0;
		}	
	}
	
	drupal_set_message($count." Users Synced to Customer.io");
	
	drupal_goto('admin/config/system/cio/config');
}