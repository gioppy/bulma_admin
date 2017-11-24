<?php

function bulma_admin_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<div class="tabs is-toggle"><ul>';
    $variables['primary']['#suffix'] = '</ul></div>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Alter menu local task
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }
    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));
  }

  return '<li' . (!empty($variables['element']['#active']) ? ' class="is-active"' : '') . '>' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}

function bulma_admin_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  $link['localized_options']['attributes']['class'] = array('button', 'is-info', 'is-inverted', 'is-outlined');

  $output = '<li class="control">';
  if (isset($link['href'])) {
    $output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $link['title'];
  }
  else {
    $output .= check_plain($link['title']);
  }
  $output .= "</li>\n";

  return $output;
}

/**
 * alter management menu tree
 * @param $variables array
 * @return string
 */
function bulma_admin_menu_tree__management(&$variables){
  $level = 1;
  foreach ($variables['#tree'] as $key => $item) {
    if (is_int($key)) {
      if ($item['#original_link']['p3'] == 0) {
        $level = 0;
      }
    }
  }
  return $level == 0 ? '<ul class="navbar-start ba__navbar-start--small">' . $variables['tree'] . '</ul>' : '<ul class="navbar-dropdown">' . $variables['tree'] . '</ul>';
}

/**
 * alter management menu links
 * @param $variables array
 * @return string
 */
function bulma_admin_menu_link__management($variables){
  $element = $variables['element'];
  $sub_menu = '';
  $output = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $element['#localized_options']['attributes']['class'][] = 'navbar-item';

  // First level item
  if ($element['#original_link']['p1'] != 0 && $element['#original_link']['p2'] != 0 && $element['#original_link']['p3'] == 0) {
    $element['#attributes']['class'][] = 'has-dropdown';
    $element['#attributes']['class'][] = 'navbar-item';
  }

  if($element['#original_link']['p2'] != 0){
    if ($element['#original_link']['p3'] == 0 && $element['#original_link']['has_children'] == 1) {
      $element['#attributes']['class'][] = 'has-dropdown is-hoverable';
      $element['#localized_options']['attributes']['class'][] = 'navbar-link';
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    } else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    }
  }else{
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  }

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Build management menu for navbar
 * TODO: make all items visibile, see admin_menu
 * @return array
 */
function _bulma_admin_build_management_menu() {
  $data = menu_tree_all_data('management');
  reset($data);
  $key = key($data);
  return menu_tree_output($data[$key]['below']);
}