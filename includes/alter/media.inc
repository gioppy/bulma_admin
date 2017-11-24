<?php

/**
 * Implements theme_media_widget()
 *
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_media_widget($variables) {
  $element = $variables['element'];
  $output = '';

  // Merge provided attributes with existing ones.
  // The "form-media" class is required for proper Ajax functionality.
  $attributes = array(
    'class' => array("media-widget", "form-media", "clearfix", "test"),
  );
  if (!empty($element['#attributes'])) {
    $attributes = array_merge_recursive($attributes, $element['#attributes']);
  }
  if (!empty($element['#id'])) {
    $attributes = array_merge_recursive($attributes, array('id' => $element['#id'] . '--widget'));
  }

  $element['browse_button']['#attributes']['class'][] = 'is-dark';
  $element['edit']['#attributes']['class'][] = 'is-info';

  // Render attributes into div in one go.
  $output .= '<div ' . drupal_attributes($attributes) . '>';
  $output .= drupal_render_children($element);
  $output .= '</div>';

  return $output;
}