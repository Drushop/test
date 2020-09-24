<?php
/**
 * @file
 * Provides functions related with blocks.
 */

/**
 * Implements hook_block_info().
 */
function search_api_devel_block_info() {
  $blocks = array();

  $blocks['search_api_query_debug'] = array(
    'info' => 'Search API Devel: Query Debugger',
    'cache' => DRUPAL_NO_CACHE,
    'region' => 'content',
    'status' => 1,
    'weight' => -1,
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function search_api_devel_block_view($delta = '') {
  if ($delta == 'search_api_query_debug') {
    // Retrieve data from static.
    $results = &drupal_static('search_api_devel_search_api_results', array());

    // If we have debug information, display.
    if ($results) {
      return array(
        'subject' => t('Search API Devel query debugger'),
        'content' => kprint_r($results, TRUE),
      );
    }
  }
}

