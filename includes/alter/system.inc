<?php

/**
 * Alter theme_status_messages()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status = array(
    'status' => array(
      'title' => t('Status message'),
      'class' => 'is-success'
    ),
    'error' => array(
      'title' => t('Error message'),
      'class' => 'is-danger'
    ),
    'warning' => array(
      'title' => t('Warning message'),
      'class' => 'is-warning'
    ),
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= "<div class=\"message " . $status[$type]['class'] . "\">\n";
    $output .= "<div class=\"message-body\">\n";
    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status[$type]['title'] . "</h2>\n";
    }
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= reset($messages);
    }
    $output .= "</div>\n";
    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Alter theme_breadcrumb()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb has-bullet-separator is-small">';
    $output .= '<ul>';
    foreach ($breadcrumb as $piece) {
      $output .= '<li>' . $piece . '</li>';
    }
    $output .= '</ul>';
    $output .= '</div>';
    return $output;
  }
}

function bulma_admin_admin_block($variables) {
  $block = $variables['block'];
  $output = '';

  // Don't display the block if it has no content to display.
  if (empty($block['show'])) {
    return $output;
  }

  $output .= '<div class="panel">';
  if (!empty($block['title'])) {
    $output .= '<h3 class="panel-heading">' . $block['title'] . '</h3>';
  }
  if (!empty($block['content'])) {
//    $output .= '<div class="panel-block">' . $block['content'] . '</div>';
    $output .= $block['content'];
  }
  else {
    $output .= '<div class="description">' . $block['description'] . '</div>';
  }
  $output .= '</div>';

  return $output;
}

function bulma_admin_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';

  if (!empty($content)) {
    $class = 'admin-list';
    if ($compact = system_admin_compact_mode()) {
      $class .= ' compact';
    }
    // $output .= '<dl class="' . $class . '">';
    foreach ($content as $item) {
      // $output .= '<dt class="panel-block">' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<div class="panel-block ba__panel-block--inset">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (!$compact && isset($item['description'])) {
        // $output .= '<dd class="panel-block">' . filter_xss_admin($item['description']) . '</dd>';
        $output .= '<p class="help">' . filter_xss_admin($item['description']) . '</p>';
      }
      $output .= '</div>';
    }
    // $output .= '</dl>';
  }
  return $output;
}

/**
 * Alter theme_table()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_table($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }

  $attributes['class'][] = 'table';
  $attributes['class'][] = 'is-fullwidth';
  $attributes['class'][] = 'is-striped';
  $attributes['class'][] = 'is-narrow';

  $output = '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }
    $rows[] = array(array('data' => $empty, 'colspan' => $header_count, 'class' => array('empty', 'message')));
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there are rows
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    foreach ($rows as $number => $row) {
      // Check if we're dealing with a simple or complex row
      if (isset($row['data'])) {
        $cells = $row['data'];
        $no_striping = isset($row['no_striping']) ? $row['no_striping'] : FALSE;

        // Set the attributes array and exclude 'data' and 'no_striping'.
        $attributes = $row;
        unset($attributes['data']);
        unset($attributes['no_striping']);
      }
      else {
        $cells = $row;
        $attributes = array();
        $no_striping = FALSE;
      }
      if (count($cells)) {
        // Add odd/even class
        if (!$no_striping) {
          $class = $flip[$class];
          $attributes['class'][] = $class;
        }

        // Build row
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  $output .= "</table>\n";
  return $output;
}

/**
 * Alter theme_tablesort_indicator()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_tablesort_indicator($variables) {
  if ($variables['style'] == "asc") {
    $asc = array('#markup' => '<span class="icon"><i class="fa fa-angle-down" aria-hidden="true"></i></span>');
    return drupal_render($asc);
  } else {
    $desc = array('#markup' => '<span class="icon"><i class="fa fa-angle-up" aria-hidden="true"></i></span>');
    return drupal_render($desc);
  }
}

/**
 * Alter theme_container()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_container($variables) {
  $element = $variables['element'];
  // Ensure #attributes is set.
  $element += array('#attributes' => array());

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {
    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }
    // Add the 'form-wrapper' class.
    $element['#attributes']['class'][] = 'form-wrapper';
  }

  if (isset($element['#type'])) {
    if ($element['#type'] == 'actions') {
      $element['#attributes']['class'][] = 'field';
      $element['#attributes']['class'][] = 'is-grouped';
    };
  }

  return '<div' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>';
}

/**
 * Alter theme_vertical_tabs()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_vertical_tabs($variables) {
  $element = $variables['element'];
  // Add required JavaScript and Stylesheet.
  drupal_add_library('system', 'drupal.vertical-tabs');

  $output = '<hr><div class="columns ba-vertical-tabs"><h2 class="element-invisible">' . t('Vertical Tabs') . '</h2>';
  $output .= '<div class="vertical-tabs-panes column notification">' . $element['#children'] . '</div></div>';
  return $output;
}

/**
 * Alter theme_status_report()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_status_report($variables) {
  $requirements = $variables['requirements'];
  $severities = array(
    REQUIREMENT_INFO => array(
      'title' => t('Info'),
      'class' => 'info',
    ),
    REQUIREMENT_OK => array(
      'title' => t('OK'),
      'class' => 'ok is-success',
    ),
    REQUIREMENT_WARNING => array(
      'title' => t('Warning'),
      'class' => 'warning is-warning',
    ),
    REQUIREMENT_ERROR => array(
      'title' => t('Error'),
      'class' => 'error is-danger',
    ),
  );
  $output = '<table class="system-status-report">';

  foreach ($requirements as $requirement) {
    if (empty($requirement['#type'])) {
      $severity = $severities[isset($requirement['severity']) ? (int) $requirement['severity'] : REQUIREMENT_OK];
      $severity['icon'] = '<div title="' . $severity['title'] . '"><span class="element-invisible">' . $severity['title'] . '</span></div>';
      // The requirement's 'value' key is optional, provide a default value.
      $requirement['value'] = isset($requirement['value']) ? $requirement['value'] : '';

      // Output table row(s)
      if (!empty($requirement['description'])) {
        $output .= '<tr class="notification ' . $severity['class'] . ' merge-down"><td class="status-icon">' . $severity['icon'] . '</td><td class="status-title">' . $requirement['title'] . '</td><td class="status-value">' . $requirement['value'] . '</td></tr>';
        $output .= '<tr class="notification ' . $severity['class'] . ' merge-up"><td colspan="3" class="status-description">' . $requirement['description'] . '</td></tr>';
      }
      else {
        $output .= '<tr class="notification ' . $severity['class'] . '"><td class="status-icon">' . $severity['icon'] . '</td><td class="status-title">' . $requirement['title'] . '</td><td class="status-value">' . $requirement['value'] . '</td></tr>';
      }
    }
  }

  $output .= '</table>';
  return $output;
}

/**
 * Alter theme_mark()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_mark($variables) {
  $type = $variables['type'];
  global $user;
  if ($user->uid) {
    if ($type == MARK_NEW) {
      return ' <span class="marker tag is-success is-rounded">' . t('new') . '</span>';
    }
    elseif ($type == MARK_UPDATED) {
      return ' <span class="marker tag is-dark is-rounded">' . t('updated') . '</span>';
    }
  }
}

/**
 * Alter theme_pager_link()
 * @param $variables array
 *
 * @return string
 */
function bulma_admin_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  $attributes['class'] = array('pagination-link');
  return '<a' . drupal_attributes($attributes) . '>' . check_plain($text) . '</a>';
}

/**
 * Alter theme_pager()
 * @param $variables array
 *
 * @return string
 *
 * TODO: fix styles
 */
function bulma_admin_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : t('« first')), 'element' => $element, 'parameters' => $parameters));
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('‹ previous')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('next ›')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : t('last »')), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '<span class="pagination-ellipsis">…</span>',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current'),
            'data' => '<span class="pagination-link is-current">' . $i . '</span>',
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '<span class="pagination-ellipsis">…</span>',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }

    $element = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('pagination'),
        'role' => 'navigation',
        'aria-label' => 'pagination'
      ),
      'title' => array(
        '#markup' => '<h2 class="element-invisible">' . t('Pages') . '</h2>'
      ),
      'items' => array(
        '#markup' => theme('item_list', array(
            'items' => $items,
            'attributes' => array('class' => array('pagination-list')),
          ))
      )
    );

    return drupal_render($element);
  }
}
