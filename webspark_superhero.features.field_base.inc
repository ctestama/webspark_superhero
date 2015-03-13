<?php
/**
 * @file
 * webspark_superhero.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function webspark_superhero_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_webspark_superhero_items'
  $field_bases['field_superhero_items'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_superhero_items',
    'foreign keys' => array(
      'fid' => array(
        'columns' => array(
          'fid' => 'fid',
        ),
        'table' => 'file_managed',
      ),
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'webspark_superhero',
    'settings' => array(
      'default_image' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'webspark_superhero',
  );

  // Exported field_base: 'field_optionset'
  $field_bases['field_superhero_optionset'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_superhero_optionset',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );
   

  return $field_bases;
}
