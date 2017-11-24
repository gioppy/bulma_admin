<?php

/**
 * @file
 * theme-settings.php
 *
 * Provides theme settings for Bootstrap based themes when admin theme is not.
 *
 * @see ./includes/settings.inc
 */

/**
 * Implements hook_form_FORM_ID_alter().
 * @param $form array
 * @param $form_state array
 * @param null | string | integer $form_id
 */
function bulma_admin_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
  $form['cdn'] = array(
    '#type' => 'fieldset',
    '#title' => t('CDN'),
  );

  $form['cdn']['official'] = array(
    '#type' => 'textfield',
    '#title' => t('Official CDN'),
    '#default_value' => 'https://cdnjs.cloudflare.com/ajax/libs/bulma/0.5.1/css/bulma.min.css',
    '#disabled' => TRUE,
  );
}