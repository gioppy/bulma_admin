<?php

/**
 * Alter theme_links__ctools_dropbutton()
 * @param $vars array
 *
 * @return string
 */
function bulma_admin_links__ctools_dropbutton($vars) {
  // Check to see if the number of links is greater than 1;
  // otherwise just treat this like a button.
  if (!empty($vars['links'])) {
    $is_drop_button = (count($vars['links']) > 1);

    // Add needed files
    if ($is_drop_button) {
      ctools_add_js('dropbutton');
      ctools_add_css('dropbutton');
    }
    //ctools_add_css('button');

    // Provide a unique identifier for every button on the page.
    static $id = 0;
    $id++;

    // Wrapping div
    $class = 'ctools-no-js';
    $class .= ($is_drop_button) ? ' ctools-dropbutton' : '';
    $class .= ' dropdown is-hoverable is-right';
    $class .= ' ctools-button';
    if (!empty($vars['class'])) {
      $class .= ($vars['class']) ? (' ' . implode(' ', $vars['class'])) : '';
    }

    $output = '';

    $output .= '<div class="' . $class . '" id="ctools-button-' . $id . '">';

    $vars['title'] = ($vars['title'] ? check_plain($vars['title']) : t('actions'));
    // Add a twisty if this is a dropbutton
    if ($is_drop_button) {
      $output .= '<div class="dropdown-trigger">';
      if ($vars['image']) {
        $output .= '<a href="#" class="ctools-twisty ctools-image">' . $vars['title'] . '</a>';
      }
      else {
        $output .= '<button type="button" class="button is-info is-outlined is-small"><span>' . $vars['title'] . '</span><span class="icon is-small">
        <i class="fa fa-angle-down" aria-hidden="true"></i>
      </span></button>';
      }
      $output .= '</div>'; // dropdown-trigger
    } else {
      $output .= '<button type="button" class="button is-info is-outlined is-small"><span>' . $vars['title'] . '</span><span class="icon is-small">
        <i class="fa fa-angle-down" aria-hidden="true"></i>
      </span></button>';
    }

    // The button content
    $output .= '<div class="dropdown-menu"><div class="dropdown-content">';

    // Check for attributes. theme_links expects an array().
    $vars['attributes'] = (!empty($vars['attributes'])) ? $vars['attributes'] : array();

    // Remove the inline and links classes from links if they exist.
    // These classes are added and styled by Drupal core and mess up the default
    // styling of any link list.
    if (!empty($vars['attributes']['class'])) {
      $classes = $vars['attributes']['class'];
      foreach ($classes as $key => $class) {
        if ($class === 'inline' || $class === 'links') {
          unset($vars['attributes']['class'][$key]);
        }
      }
    }

    foreach ($vars['links'] as $key => $link) {
      $vars['links'][$key]['attributes']['class'][] = 'dropdown-item';

      if ($key == 'delete') {
        $vars['links']['remove'] = $vars['links']['delete'];
        unset($vars['links']['delete']);
      }
    }

    // Call theme_links to render the list of links.
    $output .= theme_links(array('links' => $vars['links'], 'attributes' => $vars['attributes'], 'heading' => ''));
    $output .= '</div></div>'; // ctools-content
    $output .= '</div>'; // ctools-dropbutton
    return $output;
  }
  else {
    return '';
  }
}