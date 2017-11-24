<?php

bulma_admin_include('bulma_admin', 'includes/alter/system.inc');
bulma_admin_include('bulma_admin', 'includes/alter/menu.inc');
bulma_admin_include('bulma_admin', 'includes/alter/form.inc');
bulma_admin_include('bulma_admin', 'includes/alter/ctools.inc');
bulma_admin_include('bulma_admin', 'includes/alter/media.inc');

if (module_exists('webform')) {
  bulma_admin_include('bulma_admin', 'includes/alter/webform.inc');
}

function bulma_admin_form_alter(&$form, &$form_state, $form_id) {
  //dpm($form['actions']);
}

/**
 * Implements hook_css_alter()
 * @param $css array
 */
function bulma_admin_css_alter(&$css) {
  unset($css[drupal_get_path('module', 'system') . '/system.menus.css']);

  if (isset($css['misc/vertical-tabs.css'])) {
    unset($css['misc/vertical-tabs.css']);
  }

  $theme_path = drupal_get_path('theme', 'bulma_admin');

  // Add CDN assets, if any.
  $cdn_asset = bulma_admin_setting('cdn');
  $css[$cdn_asset] = array(
    'data' => $cdn_asset,
    'type' => 'external',
    'every_page' => TRUE,
    'media' => 'all',
    'preprocess' => FALSE,
    'group' => CSS_THEME,
    'browsers' => array('IE' => TRUE, '!IE' => TRUE),
    'weight' => -3,
  );

  //TODO: add FA to theme-settings
  $css['fa'] = array(
    'data' => 'https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css',
    'type' => 'external',
    'every_page' => TRUE,
    'media' => 'all',
    'preprocess' => FALSE,
    'group' => CSS_THEME,
    'browsers' => array('IE' => TRUE, '!IE' => TRUE),
    'weight' => -4,
  );
}

/**
 * Implements hook_js_alter()
 * @param $js array
 */
function bulma_admin_js_alter(&$js) {
  if (isset($js['misc/vertical-tabs.js'])) {
    unset($js['misc/vertical-tabs.js']);
  }

  if (isset($js['misc/collapse.js'])) {
    unset($js['misc/collapse.js']);
  }
}

/**
 * Implements hook_preprocess_page()
 * @param $variables array
 */
function bulma_admin_preprocess_page(&$variables) {
  $tree = _bulma_admin_build_management_menu();
  $variables['management_menu'] = drupal_render($tree);
}

/**
 * Implements hook_form_FORM_ID_alter()
 * @param $form array
 * @param $form_state array
 * @param $form_id string
 */
function bulma_admin_form_views_ui_add_form_alter(&$form, &$form_state, $form_id) {
  $form['save']['#prefix'] = '<div class="field is-grouped">';
  $form['cancel']['#suffix'] = '</div>';
}

/**
 * Implements hook_form_FORM_ID_alter()
 * @param $form array
 * @param $form_state array
 * @param $form_id string
 */
function bulma_admin_form_views_ui_add_item_form_alter(&$form, &$form_state, $form_id) {
  $form['buttons']['#prefix'] = '<div class="clearfix"><div class="form-buttons field is-grouped">';
  $form['buttons']['#suffix'] = '</div></div>';
}

/**
 * Implements hook_form_FORM_ID_alter()
 * @param $form array
 * @param $form_state array
 * @param $form_id string
 */
function bulma_admin_form_views_ui_config_item_form_alter(&$form, &$form_state, $form_id) {
  $form['buttons']['#prefix'] = '<div class="clearfix"><div class="form-buttons field is-grouped">';
  $form['buttons']['#suffix'] = '</div></div>';
}

/**
 * Implements hook_form_FORM_ID_alter()
 * @param $form array
 * @param $form_state array
 * @param $form_id string
 */
function bulma_admin_form_views_ui_edit_display_form_alter(&$form, &$form_state, $form_id) {
  $form['buttons']['#prefix'] = '<div class="clearfix"><div class="form-buttons field is-grouped">';
  $form['buttons']['#suffix'] = '</div></div>';
}

/**
 * Implements hook_form_FORM_ID_alter()
 * @param $form array
 * @param $form_state array
 * @param $form_id string
 */
function bulma_admin_form_views_ui_edit_form_alter(&$form, &$form_state, $form_id) {
  // grouping add display buttons
  $form['displays']['top']['add_display']['#prefix'] = '<div class="field is-grouped">';
  $form['displays']['top']['add_display']['#suffix'] = '</div>';

  // fix add display buttons style
  foreach ($form['displays']['top']['add_display'] as $key => $button) {
    if (isset($form['displays']['top']['add_display'][$key]['#attributes'])) {
      $form['displays']['top']['add_display'][$key]['#attributes']['class'][] = 'is-dark';
      $form['displays']['top']['add_display'][$key]['#attributes']['class'][] = 'is-outlined';
      $form['displays']['top']['add_display'][$key]['#attributes']['class'][] = 'is-small';
    }
  }

  // fix view tabs
  $form['displays']['top']['tabs']['#prefix'] = '<div class="tabs is-small is-toggle"><ul>';
  $form['displays']['top']['tabs']['#suffix'] = '</ul></div>';
}
