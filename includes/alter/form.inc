<?php

/**
 * Alter theme_form_element()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_form_element($variables) {
  $element = &$variables['element'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $attributes['class'][] = 'field';
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<div class="field-prefix control"><span class="button is-static">' . $element['#field_prefix'] . '</span></div> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <div class="field-suffix control"><span class="button is-static">' . $element['#field_suffix'] . '</span></div>' : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= $prefix || $suffix ? '<div class="field has-addons">' : '';
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      $output .= $prefix || $suffix ? '</div>' : '';
      break;

    case 'after':
      $output .= $prefix || $suffix ? '<div class="field has-addons">' : '';
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= $prefix || $suffix ? '</div>' : '';
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= $prefix || $suffix ? '<div class="field has-addons">' : '';
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      $output .= $prefix || $suffix ? '</div>' : '';
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<div class="help">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/**
 * Alter theme_form_element_label()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_form_element_label($variables) {
  $element = $variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If title and required marker are both empty, output no label.
  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required'])) {
    return '';
  }

  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);

  $attributes = array();
  $attributes['class'] = 'label';
  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after') {
    $attributes['class'] .= ' option';
  }

  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $attributes['class'] .= ' element-invisible';
  }

  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
}

/**
 * Alter theme_fieldset()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper', 'ba-fieldset', 'box'));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend class="label ba-legend"><span class="fieldset-legend">' . $element['#title'] . '</span></legend>';
  }
  $output .= '<div class="fieldset-wrapper">';
  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}

/**
 * Alter theme_textfield()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, array('id', 'name', 'value', 'size', 'maxlength'));
  _form_set_class($element, array('form-text', 'input'));

  $extra = '';
  if ($element['#autocomplete_path'] && !empty($element['#autocomplete_input'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#autocomplete_input']['#id'];
    $attributes['value'] = $element['#autocomplete_input']['#url_value'];
    $attributes['disabled'] = 'disabled';
    $attributes['class'][] = 'autocomplete';
    $extra = '<div class="control"><input' . drupal_attributes($attributes) . ' /></div>';
  }

  // fix media field elements order
  if (isset($element['#name'])) {
    if (preg_match('/^media/', $element['#name'])) {
      $output = '<input' . drupal_attributes($element['#attributes']) . ' />';
    } else {
      $output = '<div class="control"><input' . drupal_attributes($element['#attributes']) . ' /></div>';
    }
  }

  return $output . $extra;
}

/**
 * Alter theme_textarea()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_textarea($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea', 'textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    drupal_add_library('system', 'drupal.textarea');
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Alter theme_select()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_select($variables){
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));
  $classes = 'select';

  // TODO: make better classes
  if (isset($element['#attributes']['multiple'])) {
    $classes .= ' is-multiple';
  }

  _form_set_class($element, array('form-select'));

  return '<div class="control"><div class="' . $classes . '"><select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select></div></div>';
}

/**
 * Alter theme_file()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_file($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'file';
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-file', 'file-input'));

  // TODO: review on simple field image
  return '<div class="file"><label class="file-label field is-grouped"><input' . drupal_attributes($element['#attributes']) . ' /><span class="file-cta control"><span class="file-icon"><i class="fa fa-upload"></i></span><span class="file-label">' . t('Choose a file...') . '</span></span></label></div>';
}

function bulma_admin_file_widget($variables) {
  $element = $variables['element'];
  $output = '';

  // The "form-managed-file" class is required for proper Ajax functionality.
  $output .= '<div class="file file-widget form-managed-file clearfix">';
  if ($element['fid']['#value'] != 0) {
    // Add the file size after the file name.
    $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
  }
  $output .= drupal_render_children($element);
  $output .= '</div>';

  return $output;
}

/**
 * Alter theme_image_widget()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_image_widget($variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="image-widget form-managed-file clearfix file">';

  if (isset($element['preview'])) {
    $output .= '<div class="image-preview media">';
    $output .= '<figure class="media-left">';
    $output .= drupal_render($element['preview']);
    $output .= '</figure>';

    $output .= '<div class="image-widget-data media-content">';
    if ($element['fid']['#value'] != 0) {
      //TODO: make better alignment
      $element['filename']['#prefix'] = '<div class="field is-grouped">';
      $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
      $element['filename']['#suffix'] = '</div>';
    }
    $output .= drupal_render_children($element);
    $output .= '</div>';
    $output .= '</div>';

    $output .= '</div>';
  } else {
    $output .= '<label class="image-widget-data file-label field is-grouped">';
    if ($element['fid']['#value'] != 0) {
      $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
    }
    $output .= drupal_render_children($element);
    $output .= '</label>';
    $output .= '</div>';
  }

  return $output;
}

/**
 * Alter theme_file_managed_file()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_file_managed_file($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-managed-file';
  $attributes['class'][] = 'field';
  $attributes['class'][] = 'is-grouped';

  // This wrapper is required to apply JS behaviors and CSS styling.
  $output = '';
  $output .= '<div class="file">';
  $output .= '<label class="file-label field is-grouped">';
  $output .= '<div' . drupal_attributes($attributes) . '>';
  $output .= drupal_render_children($element);
  $output .= '</label>';
  $output .= '</div>';
  $output .= '</div>';
  return $output;
}

/**
 * Alter theme_button()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_button($variables) {
  $element = $variables['element'];
  $text = $element['#value'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'button';

  switch ($text) {
    case 'Save':
    case 'Save blocks':
    case 'Save content type':
    case 'Save configuration':
    case 'Add':
    case 'Save changes':
    case 'Create new style':
    case 'Update style':
    case 'Add format':
    case 'Create':
    case 'Save & exit':
    case 'Add and configure fields':
    case 'Add and configure filter criteria':
    case 'Add and configure sort criteria':
    case 'Add and configure contextual filters':
    case 'Add and configure relationships':
    case 'Add and configure no results behavior':
    case 'Apply':
    case 'Ok':
      $element['#attributes']['class'][] = 'is-primary';
      break;

    case 'Preview':
    case 'Filter':
    case 'Update':
    case 'Run cron':
    case 'Continue & edit':
    case 'Update preview':
    case 'Change theme':
    case 'Rescan template files':
    case 'Next':
    case 'Previous':
      $element['#attributes']['class'][] = 'is-info';
      break;

    case 'Upload':
    case 'Reset':
      $element['#attributes']['class'][] = 'is-dark';
      break;

    case 'Override defaults':
    case 'Re-index site':
    case 'Clear all caches':
    case 'Cancel':
      $element['#attributes']['class'][] = 'is-warning';
      break;

    case 'Remove':
    case 'Delete':
    case 'Delete content type':
      $element['#attributes']['class'][] = 'is-danger';
      break;

    default:
      $element['#attributes']['class'][] = 'is-light';
      break;
  }

  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  if (isset($element['#submit'])) {
    if (preg_match('/^media/', $element['#submit'][0])) {
      return '<input' . drupal_attributes($element['#attributes']) . ' />';
    } else {
      return '<div class="control"><input' . drupal_attributes($element['#attributes']) . ' /></div>';
    }
  } else {
    return '<div class="control"><input' . drupal_attributes($element['#attributes']) . ' /></div>';
  }

}