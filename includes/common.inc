<?php

/**
 * Includes a theme file.
 *
 * @param string $theme
 *   Name of the theme to use for base path.
 * @param string $path
 *   Path relative to $theme.
 */
function bulma_admin_include($theme, $path) {
  static $themes = array();
  if (!isset($themes[$theme])) {
    $themes[$theme] = drupal_get_path('theme', $theme);
  }
  if ($themes[$theme] && ($file = DRUPAL_ROOT . '/' . $themes[$theme] . '/' . $path) && file_exists($file)) {
    include_once $file;
  }
}

/**
 * Retrieves a setting for the current theme or for a given theme.
 *
 * This is a wrapper for theme_get_setting(), ensuring to use deprecated
 * setting values instead.
 *
 * @param string $name
 *   The name of the setting to be retrieved.
 * @param string $theme
 *   The name of a given theme; defaults to the currently active theme.
 * @param string $prefix
 *   The prefix used on the $name of the setting, this will be appended with
 *   "_" automatically if set.
 *
 * @return mixed
 *   The value of the requested setting, NULL if the setting does not exist.
 *
 * @see theme_get_setting()
 */
function bulma_admin_setting($name, $theme = NULL, $prefix = 'bulma_admin') {
  $prefix = !empty($prefix) ? $prefix . '_' : '';
  $setting = theme_get_setting($prefix . $name, $theme);
  /*switch ($prefix . $name) {
    case 'bulma_admin_cdn':
      $deprecated = theme_get_setting('bootstrap_cdn', $theme);
      if (isset($deprecated)) {
        $setting = empty($deprecated) ? '' : 'jsdelivr';
      }
      break;

  }*/
  return $setting;
}
