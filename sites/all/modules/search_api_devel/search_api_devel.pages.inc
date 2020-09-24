<?php
/**
 * @file
 * Provides functions, used as menu callbacks.
 */

/**
 * Immediately reindex entity by search_api.
 */
function search_api_devel_reindex_entity($type, $id) {
  $indexes = search_api_index_load_multiple(FALSE);
  if ($type == 'node') {
    foreach ($indexes as $index) {
      if ($index->enabled) {
        search_api_index_specific_items($index, array($id));
      }
    }

    drupal_set_message(t('Node with nid @id was successfully indexed in all enabled indexes.', array('@id' => $id)));
    drupal_goto("node/$id");
  }
}

/**
 * Main callback of module.
 */
function search_api_devel_page_form($form, $form_state, $type, $id) {
  $form = array();

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => '',
  );

  $form['entity_type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );

  $form['entity_id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );

  $form['actions']['#type'] = 'container';
  if ($entity_indexed_data = search_api_devel_get_entity_indexed_data($type, $id)) {
    $form['actions']['reindex'] = array(
      '#type' => 'submit',
      '#value' => t('Reindex'),
    );

    $form['info']['#markup'] = '';
    $has_correct_data = FALSE;
    // Processing indexed data to display it.
    foreach ($entity_indexed_data as $index_id => $_data) {
      $index_link = l($_data['index']->machine_name, "admin/config/search/search_api/index/$index_id/view");
      $form['info']['#markup'] .= '<h4>' . t('Index with machine name: !index_link', array('!index_link' => $index_link)) . '</h4>';
      if (!empty($_data['error'])) {
        $form['info']['#markup'] .= $_data['error'];
      }
      else {
        $has_correct_data = TRUE;
        $form['info']['#markup'] .= kdevel_print_object($_data['data']);
      }
    }


    // If at least one index has current node and we didnt found any issues -
    // user can remove node from index.
    if ($has_correct_data) {
      $form['actions']['clear'] = array(
        '#type' => 'submit',
        '#value' => t('Remove from indexes'),
      );
    }
  }
  else {
    $form['info']['#markup'] = t('This entity is not indexed yet.');
  }
  $form['info']['#markup'] .= '<br /><br />';

  return $form;
}

/**
 * Submit handler for search_api_devel_page_form.
 */
function search_api_devel_page_form_submit($form, $form_state) {
  $indexes = search_api_index_load_multiple(FALSE, array('item_type' => $form_state['values']['entity_type'], 'enabled' => 1));

  // Reindex.
  if ($form_state['values']['op'] == t('Reindex')) {
    if (!empty($indexes)) {
      foreach ($indexes as $index) {
        search_api_index_specific_items($index, array($form_state['values']['entity_id']));
      }
      drupal_set_message(t('If entity still displayed as "not found", just wait a little time or check the option "Index items immediately" in the settings of your index.'), 'warning');
    }
  }
  // Remove from indexes.
  elseif ($form_state['values']['op'] == t('Remove from indexes')) {
    if (!empty($indexes)) {
      foreach ($indexes as $index) {
        search_api_devel_remove_entity_from_index($index, array($form_state['values']['entity_id']));
      }
      drupal_set_message(t('Sometimes servers need some time to remove entity from index. If entity still displayed as indexed, just wait several seconds and update this page to ensure that entity has been removed from index.'), 'warning');
    }
  }
}
