<?php
/**
 * @file
 * webspark_superhero.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function webspark_superhero_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fieldable_panels_pane-spotlight-field_basic_spotlight_items'
  $field_instances['fieldable_panels_pane-webspark_superhero-field_superhero_items'] = array(
    'bundle' => 'webspark_superhero',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'webspark_superhero',
        'settings' => array(
          'image_style' => 'webspark_hero',
        ),
        'type' => 'webspark_superhero',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_superhero_items',
    'label' => 'Items',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'general',
      'file_extensions' => 'jpg jpeg gif png mp4 ogg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'webspark_superhero',
      'settings' => array(),
      'type' => 'webspark_superhero_form',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'fieldable_panels_pane-webspark_superhero
  $field_instances['fieldable_panels_pane-webspark_superhero-field_superhero_optionset'] = array(
    'bundle' => 'webspark_superhero',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'fieldable_panels_pane',
    'field_name' => 'field_superhero_optionset',
    'label' => 'Optionset',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'apply_chosen' => '',
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
      ),
      'type' => 'options_select',
      'weight' => -5,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.

  t('Items');


  return $field_instances;
}
