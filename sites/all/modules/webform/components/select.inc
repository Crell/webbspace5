<?php
// $Id: select.inc,v 1.18.2.19.2.22 2008/09/21 03:20:46 quicksketch Exp $

/**
 * Create a default select component.
 */
function _webform_defaults_select() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'items' => '',
      'email' => 0,
      'multiple' => NULL,
      'aslist' => NULL,
      'description' => '',
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they may
 * be overridden if desired).
 * @return
 *   An array of form items to be displayed on the edit component page.
 */
function _webform_edit_select($currfield) {
  $edit_fields = array();
  $edit_fields['extra']['items'] = array(
    '#type' => 'textarea',
    '#title' => t("Options"),
    '#default_value' => $currfield['extra']['items'],
    '#description' => t('A list of selectable options. One option per line. Key-value pairs may be entered seperated by pipes. i.e. safe_key|Some readable option') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -2,
    '#required' => TRUE,
  );
  $edit_fields['value'] = array(
    '#type' => 'textfield',
    '#title' => t("Default value"),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field. For multiple selects use commas to separate multiple defaults.') . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 256,
    '#weight' => 0,
  );
  $edit_fields['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t("Multiple"),
    '#return_value' => 'Y',
    '#default_value' => ($currfield['extra']['multiple'] == 'Y' ? TRUE : FALSE),
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
  );
  $edit_fields['extra']['aslist'] = array(
    '#type' => 'checkbox',
    '#title' => t("Listbox"),
    '#return_value' => 'Y',
    '#default_value' => ($currfield['extra']['aslist'] == 'Y' ? TRUE : FALSE),
    '#description' => t('Check this option if you want the select component to be of listbox type instead of radiobuttons or checkboxes.'),
  );
  $edit_fields['extra']['email'] = array(
    '#type' => 'checkbox',
    '#title' => t("E-mail a submission copy"),
    '#return_value' => 1,
    '#default_value' => $currfield['extra']['email'],
    '#description' => t('Check this option if this component contains an e-mail address that should get a copy of the submission. Emails are sent individually so other emails will not be shown to the recipient.') ." ".
                      t('To use the option with a select component, you must use key-value pairs seperated by pipes. i.e. user@example.com|Sample user.'),
  );
  return $edit_fields;
}

function _webform_edit_validate_select($form_values) {
  // Currently no validation for selects.
  // TODO: Validate e-mail addresses when used as keys?
  return true;
}

/**
 * Build a form item array containing all the properties of this component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of a form item to be displayed on the client-side webform.
 */
function _webform_render_select($component) {
  $form_item = array(
    '#title'         => $component['name'],
    '#required'      => $component['mandatory'],
    '#weight'        => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#prefix'        => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix'        => '</div>',
  );

  // Convert the user-entered options list into an array.
  $default_value = _webform_filter_values($component['value'], NULL, NULL, FALSE);
  $options = _webform_select_options($component['extra']['items']);

  if ($component['extra']['aslist'] == 'Y' && $component['extra']['multiple'] != 'Y') {
    $options = array('' => t('select...')) + $options;
  }

  // Set the component options.
  $form_item['#options'] = $options;

  // Set the default value.
  if ($default_value != '') {
    // Convert default value to a list if necessary.
    if ($component['extra']['multiple'] == 'Y') {
      $varray = array_filter(explode(',', $default_value));
      foreach ($varray as $key => $v) {
        $form_item['#default_value'][] = $v;
      }
    }
    else {
      $form_item['#default_value'] = $default_value;
    }
  }

  if ($component['extra']['aslist'] == 'Y') {
    // Set display as a select list:
    $form_item['#type'] = 'select';
    if ($component['extra']['multiple'] == 'Y') {
      $form_item['#multiple'] = TRUE;
    }
  }
  else {
    if ($component['extra']['multiple'] == 'Y') {
      // Set display as a checkbox set.
      $form_item['#type'] = 'checkboxes';
      // Drupal 5 hack to correct default values.
      $form_item['#process'] = array('webform_expand_checkboxes' => array());
    }
    else {
      // Set display as a radio set.
      $form_item['#type'] = 'radios';
    }
  }
  return $form_item;
}

/**
 * Drupal 5 hack that properly sets checkbox values in multistep forms. The
 * checkboxes element get the proper values set by FAPI, but individual
 * checkboxes are always set to 0 in a multistep form.
 */
function webform_expand_checkboxes($element) {
  $element = expand_checkboxes($element);
  foreach (element_children($element) as $key) {
    if (empty($element['#value'][$key])) {
      $element[$key]['#value'] = 0;
    }
    else {
      $element[$key]['#value'] = 1;
    }
  }
  return $element;
}

/**
 * Display the result of a textfield submission. The output of this function
 * will be displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_submission_display_select($data, $component, $enabled = false) {
  $form_item = _webform_render_select($component);
  if ($component['extra']['multiple'] == 'Y') {
    // Set the value as an array.
    foreach ((array)$data['value'] as $key => $value) {
      $form_item['#default_value'][] = $value;
    }
  }
  else {
    // Set the value as a single string.
    foreach ((array)$data['value'] as $value) {
      $form_item['#default_value'] = $value;
    }
  }
  $form_item['#disabled'] = !$enabled;
  return $form_item;
}


/**
 * Convert FAPI 0/1 values into something saveable.
 *
 * @param $data
 *   The POST data associated with the component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Nothing.
 */
function _webform_submit_select(&$data, $component) {
  $options = drupal_map_assoc(array_flip(_webform_select_options($component['extra']['items'])));

  if (is_array($data)) {
    foreach ($data as $key => $value) {
      if ($value != '') {
        $data[$key] = $options[$key];
      }
      // Checkboxes submit a value of 0 when not checked.
      elseif ($value == 0 && $component['extra']['aslist'] != 'Y' && $component['extra']['multiple'] == 'Y') {
        unset($data[$key]);
      }
      else {
        unset($data[$key]);
      }
    }
  }
}

/**
 * Format the output of emailed data for this component.
 *
 * @param $data
 *   A string or array of the submitted data.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output to be included in the email.
 */
function theme_webform_mail_select($data, $component) {
  // Convert submitted 'safe' values to un-edited, original form.
  $options = _webform_select_options($component['extra']['items']);

  // Generate the output.
  if ($component['extra']['multiple']) {
    $output = $component['name'] .":\n";
    foreach ((array)$data as $value) {
      if ($value) {
        if ($options[$value]) {
          $output .= "    - ". $options[$value] ."\n";
        }
      }
    }
  }
  else {
    if ($options[$data]) {
      $output = $component['name'] .": ". $options[$data] ."\n";
    }
  }
  return $output;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_select($section) {
  switch ($section) {
    case 'admin/settings/webform#select_description':
      return t("Allows creation of checkboxes, radio buttons, or select menus.");
  }
}

/**
 * Calculate and returns statistics about results for this component from all
 * submission to this webform. The output of this function will be displayed
 * under the "results" tab then "analysis".
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_rows_select($component) {
  $options = _webform_select_options($component['extra']['items']);

  $query = 'SELECT data, count(data) as datacount '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND cid = %d '.
    " AND data != '0' AND data != '' ".
    ' GROUP BY data ';
  $result = db_query($query, $component['nid'], $component['cid']);
  $rows = array();
  while ($data = db_fetch_array($result)) {
    if ($options[$data['data']]) {
      $display_option = $options[$data['data']];
    }
    else {
      $display_option = $data['data'];
    }
    $rows[] = array($display_option, $data['datacount']);
  }
  return $rows;
}

/**
 * Return the result of this component's submission for display in a table. The
 * output of this function will be displayed under the "results" tab then "table".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_table_data_select($data) {
  // Set the value as a single string.
  if (is_array($data['value'])) {
    foreach ($data['value'] as $value) {
      if ($value !== '0') {
        $output .= check_plain($value) ."<br />";
      }
    }
  }
  else {
    $output = check_plain(empty($data['value']['0']) ? "" : $data['value']['0']);
  }
  return $output;
}


/**
 * Return the header information for this component to be displayed in a comma
 * seperated value file. The output of this function will be displayed under the
 * "results" tab then "download".
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_select($component) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  if ($component['extra']['multiple']) {
    $headers[0][] = '';
    $headers[1][] = $component['name'];
    $items = _webform_select_options($component['extra']['items']);
    $count = 0;
    foreach ($items as $key => $item) {
      // Empty column per sub-field in main header.
      if ($count != 0) {
        $headers[0][] = '';
        $headers[1][] = '';
      }
      $headers[2][] = $key;
      $count++;
    }
  }
  else {
    $headers[0][] = '';
    $headers[1][] = '';
    $headers[2][] = $component['name'];
  }
  return $headers;
}

/**
 * Return the result of a textfield submission. The output of this function will
 * be displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for CSV, not including either prefixed or trailing
 *   commas.
 */
function _webform_csv_data_select($data, $component) {
  $value = _webform_filter_values($component['value'], NULL, NULL, FALSE);
  $options = _webform_select_options($component['extra']['items']);
  $return = array();

  if ($component['extra']['multiple']) {
    foreach ($options as $key => $item) {
      if (in_array($key, (array)$data['value']) === TRUE) {
        $return[] = 'X';
      }
      else {
        $return[] = '';
      }
    }
  }
  else {
    $return = $data['value'][0];
  }
  return $return;
}

/**
 * Utility function to split user-entered values from new-line seperated
 * text into an array of options.
 */
function _webform_select_options($text) {
  $options = array();
  $rows = array_filter(explode("\n", _webform_filter_values(trim($text), NULL, NULL, FALSE)));
  foreach ($rows as $option) {
    $option = trim($option);
    if (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
      $options[$matches[1]] = $matches[2];
    }
    else {
      $options[$option] = $option;
    }
  }
  return $options;
}