<?php
/**************************************************************************
 *
 * @file cio.field.inc
 *
 *************************************************************************/


/**************************************************************************
 * Field Info
 *************************************************************************/
function cio_field_info()
{
	return array(
		'cio_email' => array(
			'label' => t('Customer.io Email'),
			'description' => t('This field stores an email id from Customer.io.'),
			'settings' => array('max_length' => 255),
			'instance_settings' => array('text_processing' => 0),
			'default_widget' => 'options_select',
			'default_formatter' => 'cio_email_formatter',
		)
	);
}


/**************************************************************************
 * Field Info Alter
 *************************************************************************/
function cio_field_widget_info_alter(&$info)
{
	$widgets = array(
		'options_select' => array('cio_email'),
	);
	
	foreach($widgets as $widget => $field_types)
	{
		$info[$widget]['field types'] = array_merge($info[$widget]['field types'], $field_types);
	}
}


/**************************************************************************
 * Hook element validate
 *************************************************************************
function cio_element_validate($element, &$form_state)
{
	switch($element['#type'])
	{
		case 'textfield':

			if($element['#value'] == '')
			{
				form_error($element, t('Name may not be blank.'));
			}
		break;
		
		case 'checkboxes':
			if(empty($element['#value']))
			{
				form_error($element, t('You must give at least one role access to this.'));
			}
		break;
	}
}


/**************************************************************************
 * Hook Field Is Empty
 *************************************************************************/
function cio_field_is_empty($item, $field)
{
	if(empty($item['option']) && (string) $item['option'] !== '_none')
	{
		return TRUE;
	}

	return FALSE;
}


/**************************************************************************
 * Options List
 *************************************************************************/
function cio_options_list($field, $instance, $entity_type, $entity)
{
	$emails = cio_get('emails');
	$options = array();
	$options['_none'] = t("None");
	foreach($emails->emails as $email)
	{
		$options[$email->id] = $email->name." | ID:".$email->id; 
	}
	return $options;
}


/**************************************************************************
 * Hook field formatter info
 *************************************************************************/
function cio_field_formatter_info()
{
	return array(
		'cio_email_formatter' => array(
			'label' => t('Default'),
			'field types' => array('cio_email'),
		),
	);
}


/**************************************************************************
 * Hook field formatter view
 *************************************************************************/
function cio_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display)
{	
	$element = array();
	switch($display['type'])
	{
		case 'cio_email_formatter':

			foreach ($items as $delta => $item)
			{
				if(isset($options[$item['option']]['option']))
				{
					$output = field_filter_xss($options[$item['option']]['option']);
				}
				else
				{
					$output = "Not Supplied";
				}
				
				$element[$delta] = array('#markup' => $output);
			}
		
		break;
		
		default:
	}
	
	return $element;
}


/**************************************************************************
 * Field Schema
 *************************************************************************/
function cio_field_schema($field)
{
	$schema = array();
	$schema['columns']['option'] = array(
		'type' => 'varchar',
		'length' => 25,
		'not null' => FALSE
	);

	return $schema;
}