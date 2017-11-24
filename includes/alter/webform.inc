<?php

/**
 * Alter theme_webform_conditional_groups()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_webform_conditional_groups($variables) {
  $element = $variables['element'];
  drupal_add_tabledrag('webform-conditionals-table', 'order', 'sibling', 'webform-conditional-weight');
  drupal_add_js('Drupal.theme.prototype.tableDragChangedMarker = function() { return ""; }', 'inline');
  drupal_add_js('Drupal.theme.prototype.tableDragChangedWarning = function() { return "<span>&nbsp;</span>"; }', 'inline');

  $output = '<table id="webform-conditionals-table" class="table is-fullwidth is-narrow"><tbody>';
  $element_children = element_children($element, TRUE);
  $element_count = count($element_children);
  foreach ($element_children as $index => $key) {
    if ($key === 'new') {
      $even_odd = ($index + 1) % 2 ? 'odd' : 'even';
      $element[$key]['weight']['#attributes']['class'] = array('webform-conditional-weight');
      $data = '<div class="webform-conditional-new">';
      if ($element_count === 1) {
        $data .= t('There are no conditional actions on this form.') . ' ';
      }
      $data .= t('Add a new condition:') . ' ' . drupal_render($element[$key]['new']) . drupal_render($element[$key]['remove']);
      $data .=  '</div>';
      $output .= '<tr class="webform-conditional-new-row ' . $even_odd . '">';
      $output .= '<td>' . $data . '</td>';
      $output .= '<td>' . drupal_render($element[$key]['weight']) . '</td>';
      $output .= '</tr>';
    }
    else {
      $output .= drupal_render($element[$key]);
    }
  }
  $output .= '</tbody></table>';
  $output .= drupal_render_children($element);

  return $output;
}
