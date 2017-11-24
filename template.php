<?php

/**
 * Include common functions used through out theme.
 */
include_once dirname(__FILE__) . '/includes/common.inc';

/**
 * Declare various hook_*_alter() hooks.
 *
 * hook_*_alter() implementations must live (via include) inside this file so
 * they are properly detected when drupal_alter() is invoked.
 */
bulma_admin_include('bulma_admin', 'includes/alter.inc');
