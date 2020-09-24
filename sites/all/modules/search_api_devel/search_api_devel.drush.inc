<?php

/**
 * @file
 * Drush commands for Search API devel module.
 */

/**
 * Implements hook_drush_command().
 */
function search_api_devel_drush_command() {
  $items = array();

  $items['search-api-entity-info'] = array(
    'description' => 'Display entity indexed info',
    'examples' => array(
      'drush search-api-entity-info --hide_entity_data',
      'drush sapi-entity-info --entity_type=node --entity_id=123',
    ),
    'options' => array(
      'entity_type' => 'Type of the entity',
      'entity_id' => 'Id of the entity',
      'hide_entity_data' => 'Display only index info',
    ),
    'aliases' => array('sapi-entity-info'),
    'callback' => 'drush_search_api_devel_entity_info',
  );
  $items['search-api-entity-reindex'] = array(
    'description' => 'Reindex the entity for its indexes or one certain index',
    'examples' => array(
      'drush search-api-entity-reindex',
      'drush sapi-entity-reindex --entity_type=node --entity_id=123 --index_id=-1',
    ),
    'options' => array(
      'entity_type' => 'Type of the entity',
      'entity_id' => 'Id of the entity',
      'index_id' => 'Id of the index',
    ),
    'aliases' => array('sapi-entity-reindex'),
    'callback' => 'drush_search_api_devel_entity_reindex',
  );
  $items['search-api-entity-remove-from-index'] = array(
    'description' => 'Remove the entity from its indexes or one certain index',
    'examples' => array(
      'drush search-api-entity-remove-from-index',
      'drush sapi-entity-remove --entity_type=node --entity_id=123 --index_id=-1',
    ),
    'options' => array(
      'entity_type' => 'Type of the entity',
      'entity_id' => 'Id of the entity',
      'index_id' => 'Id of the index',
    ),
    'aliases' => array('sapi-entity-remove'),
    'callback' => 'drush_search_api_devel_entity_remove_from_index',
  );

  return $items;
}

/**
 * Show the entity indexed data
 */
function drush_search_api_devel_entity_info() {
  // Receiving entity data if it was not passed through options.
  if (!$entity_type = drush_get_option('entity_type', NULL)) {
    $entity_type = drush_prompt(dt('Please, enter an entity type you want to check'));
  }
  if (!$entity_id = drush_get_option('entity_id', NULL)) {
    $entity_id = drush_prompt(dt('Please, enter an entity id you want to check'));
  }

  // If indexed data is empty - entity has no indexes for it.
  if (!$entity_indexed_data = search_api_devel_get_entity_indexed_data($entity_type, $entity_id)) {
    drush_print(t('Entity is not indexed'));
  }
  else {
    $header = array(
      'property_title' => dt('Property'),
      'value' => dt('Value'),
    );
    $rows = array(array_values($header));

    // Processing entity indexes with its data.
    foreach ($entity_indexed_data as $_data) {
      // Adding separators between different indexes.
      $rows[] = array_combine(
        array_keys($header),
        array('------------', '------------')
      );

      // Base index info.
      $index_rows = array(
        'Index ID' => $_data['index']->id,
        'Index name' => $_data['index']->name,
        'Index machine name' => $_data['index']->machine_name,
        'Status' => dt($_data['index']->enabled ? 'Enabled' : 'Disabled'),
      );

      // We will display error or data (if --hide_entity_data param was not
      // applied.)
      if (!empty($_data['error'])) {
        $index_rows['Error'] = $_data['error'];
      }
      elseif (!drush_get_option('hide_entity_data', FALSE)) {
        $index_rows['Entity indexed data'] = print_r($_data['data'], TRUE);
      }

      // Adding all stored info in the table.
      foreach ($index_rows as $property_title => $property_value) {
        $rows[] = array(
          'property_title' => dt($property_title),
          'value' => $property_value,
        );
      }
    }

    // Displaying all data.
    drush_print_table($rows, TRUE);
  }
}

/**
 * Index entity for index.
 */
function drush_search_api_devel_entity_reindex() {
  // Receiving entity data if it was not passed through options.
  if (!$entity_type = drush_get_option('entity_type', NULL)) {
    $entity_type = drush_prompt(dt('Please, enter an entity type you want to reindex'));
  }
  if (!$entity_id = drush_get_option('entity_id', NULL)) {
    $entity_id = drush_prompt(dt('Please, enter an entity id you want to reindex'));
  }
  if (!$index_id = drush_get_option('index_id', NULL)) {
    $index_id = drush_prompt(dt('Please, enter an index id if you want to affect certain index or -1 to affect all indexes'));
  }

  // Processing indexes with the 'index' action.
  _drush_search_api_devel_proceed_indexes(
    $entity_type,
    $entity_id,
    $index_id,
    'index'
  );
}

/**
 * Remove entity from the index.
 */
function drush_search_api_devel_entity_remove_from_index() {
  // Receiving entity data if it was not passed through options.
  if (!$entity_type = drush_get_option('entity_type', NULL)) {
    $entity_type = drush_prompt(dt('Please, enter an entity type you want to remove'));
  }
  if (!$entity_id = drush_get_option('entity_id', NULL)) {
    $entity_id = drush_prompt(dt('Please, enter an entity id you want to remove'));
  }
  if (!$index_id = drush_get_option('index_id', NULL)) {
    $index_id = drush_prompt(dt('Please, enter an index id if you want to affect certain index or -1 to affect all indexes'));
  }

  // Processing indexes with the 'remove' action.
  _drush_search_api_devel_proceed_indexes(
    $entity_type,
    $entity_id,
    $index_id,
    'remove'
  );

  drush_print('Sometimes servers need some time to remove entity from index. ');
  drush_print('If entity still displayed as indexed, just wait several seconds and check again.');
}

/**
 * Proceed entity indexes with action.
 *
 * @param string $entity_type
 *   Type of the entity.
 * @param integer $entity_id
 *   Entity identifier.
 * @param integer $index_id
 *   Index identifier or '-1' to proceed all entity indexes.
 * @param string $action
 *   Action to do with the entity: 'index' or 'remove'.
 */
function _drush_search_api_devel_proceed_indexes($entity_type, $entity_id, $index_id, $action) {
  $entity_indexed_data = search_api_devel_get_entity_indexed_data($entity_type, $entity_id);
  if ($index_id != -1 && empty($entity_indexed_data[$index_id])) {
    drush_print(dt('Error: entity is not stored in the index #!index_id.', array('!index_id' => $index_id)));
  }
  else {
    foreach ($entity_indexed_data as $entity_index_id => $_data) {
      // Affection only necessary index (or all).
      if ($index_id == -1 || $index_id == $entity_index_id) {
        $error_exists = !empty($_data['error']);
        $not_found_on_index_action = $error_exists && $_data['error'] != SEARCH_API_DEVEL_ERROR_ENTITY_NOT_FOUND && $action == 'index';
        // We should ignore entity absence in index if we want to index this entity.
        if ($error_exists && ($not_found_on_index_action || $action == 'remove')) {
          drush_print(dt('Error: @error (index "@index_machine_name").', array(
            '@error' => $_data['error'],
            '@index_machine_name' => $_data['index']->machine_name,
          )));
        }
        else {
          switch ($action) {
            case 'index':
              search_api_index_specific_items($_data['index'], array($entity_id));
              drush_print(dt('Entity has been reindexed in index "@index_machine_name".', array('@index_machine_name' => $_data['index']->machine_name)));
              break;
            case 'remove':
              search_api_devel_remove_entity_from_index($_data['index'], array($entity_id));
              drush_print(dt('Entity has been removed from index "@index_machine_name".', array('@index_machine_name' => $_data['index']->machine_name)));
              break;
          }
        }
      }
    }
  }
}