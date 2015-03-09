<?php

/**
* @file
* A specification for the Webspark Superhero
*/

/**
* Implements hook_field_info().
*/
function webspark_superhero_field_info() {
  return array(
    'webspark_superhero' => array(
      'label' => t('Superhero Fields'),
      'description' => t('A custom field for the Webspark Superhero'),
      'default_widget' => 'webspark_superhero_form',
      'default_formatter' => 'webspark_superhero',
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
        'default_image' => 1,
        //'default_type' => 'hero_slider',
      ),
      'instance_settings' => array(
        'file_extensions' => 'jpg jpeg gif png mp4',
        'file_directory' => 'general',
        'max_filesize' => '',
        'alt_field' => 0,
        'title_field' => 0,
        'max_resolution' => '',
        'min_resolution' => '',
      ),
    ),
  );
}



/**
*Implements hook_field_is_empty()
*
**/
function webspark_superhero_field_is_empty($item, $field) {

  if (empty($item['title']) && empty($item['link']) && empty($item['description']) && empty($item['fid'])) {
    return TRUE;
  }
  return FALSE;
}

/**
* Implements hook_field_widget_info().
*/
function webspark_superhero_field_widget_info() {
  return array(
    'webspark_superhero_form' => array(
    'label' => t('Webspark Superhero'),
    'field types' => array('webspark_superhero'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
* Implements hook_field_settings_form()
*/
function webspark_superhero_field_settings_form($field, $instance, $has_data) {
    $form = array();
    $defaults = field_info_field_settings($field['type']);
    $settings = array_merge($defaults, $field['settings']);

    $scheme_options = array();
    foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
      $scheme_options[$scheme] = $stream_wrapper['name'];
    }

    $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $settings['uri_scheme'],
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
    );

    // When the user sets the scheme on the UI, even for the first time, it's
    // updating a field because fields are created on the "Manage fields"
    // page. So image_field_update_field() can handle this change.
    $form['default_image'] = array(
    '#title' => t('Default image'),
    '#type' => 'managed_file',
    '#description' => t('If no image is uploaded, this image will be shown on display.'),
    '#default_value' => $field['settings']['default_image'],
    '#upload_location' => $settings['uri_scheme'] . '://default_images/',
    );

    $type_options = array(
      'image' => 'Image',
      'slider' => 'Slider',
      'video' => 'Video',
    );
    /*

    $form['default_type'] = array(
    '#type' => 'radios',
    '#title' => t('Default Superhero Type'),
    '#options' => $type_options,
    '#default_value' => 'Image',
    '#description' => t('Select the default Superhero type.'),
  );*/

    return $form;
  }

  /**
  * Implements hook_field_instance_settings_form()
  */
  function webspark_superhero_field_instance_settings_form($field, $instance) {
    $form = image_field_instance_settings_form($field, $instance);
    return $form;
  }

  /**
  * Implementation of hook_field_formatter_info().
  * We're going to be using our own formatter based on our own theme.
  */
  function webspark_superhero_field_formatter_info() {
    return array(
    'webspark_superhero' => array(
        'label' => t('Webspark Superhero'),
        'field types' => array('webspark_superhero'),
        'settings' => array(
            'image_style' => 'hero_bg_img',
            'caption' => FALSE,
            'optionset' => 'default',
            ),
        ),
    );
  }

  /**
  * Implements hook_field_formatter_settings_form().
  *
  */
  function webspark_superhero_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
    $display = $instance['display'][$view_mode];
    $settings = $display['settings'];
    $element = array();

    if ($display['type'] == 'webspark_superhero') {
      $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#options' => image_style_options(),
      '#default_value' => $settings['image_style'],
      '#required' => TRUE,
      );
    }
    return $element;
  }

  /**
  * Implements hook_field_formatter_settings_summary().
  *
  */
  function webspark_superhero_field_formatter_settings_summary($field, $instance, $view_mode) {

    $display = $instance['display'][$view_mode];
    $settings = $display['settings'];
    $summary = array();
    $summary[] = t('Image Style: @style', array('@style' => $settings['image_style']));
    $summary[] = !empty($settings['caption']) ? t('Display captions') : t('Do not display captions');

    return $summary;
  }

  /**
  * Implements hook_field_formatter_view().
  */
  function webspark_superhero_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $element = array();

    if (!empty($items)) {
      foreach ($items as $key => $item) {

        $tmp = $item;
        $item = array();
        $item['item'] = $tmp;

        // Setup the variables for calling theme_image_style
        $item['slide']['path'] = $item['item']['uri'];
        $item['slide']['style_name'] = $display['settings']['image_style'];
        $item['slide']['width'] = $item['item']['metadata']['width'];
        $item['slide']['height'] = $item['item']['metadata']['height'];
        $item['slide']['alt'] = $item['item']['alt'];
        $item['slide']['title'] = $item['item']['title'];

        // Render the slide item
        // If not style set, we have to call theme_image since theme_image_style
        // doesn't auto-fallback to full size image
        if (!empty($item['slide']['style_name'])) {
          // Generate the HTML for the slide
          $item['slide'] = theme('image_style', $item['slide']);
        }
        else {
          // Generate the HTML for the slide
          $item['slide'] = theme('image', $item['slide']);
        }

        // Check caption settings
        if ($display['settings']['caption']) {
          $item['caption'] = filter_xss($item['item']['title']);
        }

        $items[$key] = $item;
      }
      if(!empty($entity->field_optionset)) {
          $display['settings']['optionset'] = $entity->field_optionset['und'][0]['value'];
      } else {

      }
      $element = array(
      '#theme' => 'webspark_superhero',
      '#items' => $items,
      '#settings' => $display['settings'],
      );
    }

    return $element;
  }

  /**
  *Implements hook_form_alter().
  * This allows us to populate the proper options and default for selecting optionsets on the form
  */
  function webspark_superhero_form_alter(&$form, &$form_state, $form_id) {

    if($form_id=='fieldable_panels_panes_fieldable_panels_pane_content_type_edit_form'&&$form['#bundle']=='webspark_superhero') {
      variable_set('panopoly_magic_live_preview', 0);

      //TODO: ADD IN AJAX AND STARTING FORM STATES TO THE FORM

      //$form['field_superhero_items']['und']['#cardinality'] = 1;

      //$type_value = $form_state['values']['super_type']['und'][0]['value'];

      $form['field_superhero_items']['#states'] = array(
          'visible' => array(':input[name="super_type"]' => array(
            '!value' => false
            ),
          ),
      );

      $form['field_superhero_optionset']['#states'] = array(
          'visible' => array(':input[name="super_type"]' => array(
            'value' => 'slider'
            ),
          ),
      );

      $form['field_superhero_items']['und']['add_more']['#states'] = array(
          'visible' => array(':input[name="super_type"]' => array(
            'value' => 'slider'
            ),
          ),
      );

      $form['field_superhero_items']['und'][1]['#states'] = array(
          'visible' => array(':input[name="super_type"]' => array(
            'value' => 'slider'
            ),
          ),
      );
      //change to false for starting state

      //add wrapper for ajax form replacement
      $form['field_superhero_items']['und']['#prefix'] = '<div id="superhero_replace">';
      $form['field_superhero_items']['und']['#suffix'] = '</div>';

      $form['super_type'] = array(
        '#type' => 'radios',
        '#title' => t('Superhero Type'),
        '#description' => t('Please select a superhero type.'),
        '#options' => array(
          'image' => t('Image'),
          'slider' => t('Slider'),
          'video' =>  t('Video'),
        ),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'webspark_superhero_change_settings',
            //'wrapper' => 'superhero_replace',
            //'method' => 'replace',
            //'effect' => 'fade',
        ),
      );

      $form['hero_size'] = array(
        '#type' => 'radios',
        '#title' => t('Superhero Responsive Size'),
        '#description' => t('Change how the image/video scales (fixed-height or auto-scaled).'),
        '#options' => array(
          'sh-fixed-height' => t('Fixed Height'),
          'sh-auto-height' => t('Auto-scaled'),
        ),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'webspark_superhero_change_settings',
            //'wrapper' => 'superhero_replace',
            //'method' => 'replace',
            //'effect' => 'fade',
        ),
        '#default_value' => 'sh-auto-height',
      );

      $form['jumbo_size'] = array(
        '#type' => 'checkbox',
        '#title' => t('Jumbo'),
        '#description' => t('Would you like that jumbo-sized?'),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'webspark_superhero_change_settings',
            //'wrapper' => 'superhero_replace',
            //'method' => 'replace',
            //'effect' => 'fade',
        ),
        '#default_value' => 0,
        '#states' => array(
          'visible' => array(':input[name="hero_size"]' => array(
            'value' => 'sh-fixed-height'
            ),
          ),
        ),
      );
      unset($form['field_superhero_items']['und'][1]);


      $optionsets = array();

      ctools_include('export');
      foreach (flexslider_optionset_load_all() as $name => $optionset) {
        $optionsets[$name] = check_plain($optionset->title);
      }

      $form['field_superhero_optionset']['und']['#options'] = $optionsets;

      if(isset($form['#entity']->field_optionset) && !empty($form['#entity']->field_optionset)) {
         $form['field_optionset']['und']['#default_value'] =
         $form['#entity']->field_optionset['und'][0]['value'];
      }

      dpm($form, 'Form');
      dpm($form_state, 'form state in widget_form');

    }
  }


  /**
  *Callback for ajax form
  *
  **/
  function webspark_suphero_change_settings($form, &$form_state) {

    $commands = array();
    $commands[] = ajax_command_replace("#superhero_replace", drupal_render($form['field_superhero_items']));
    return array('#type' => 'ajax', '#commands' => $commands);

  }

  /**
  * Implements hook_field_widget_form().
  */
  function webspark_superhero_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

    if ($instance['widget']['type'] == 'webspark_superhero_form') {
      dpm($form, 'Form');
      dpm($form_state, 'form state in widget_form');
      dpm($element, 'element in form');
      // Make it a multipart form
      //$form['#attributes']['enctype'] = 'multipart/form-data';

      // Define the element
      $element['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['title']) ? $items[$delta]['title'] : NULL,
      );

      $element['link'] = array(
      '#title' => t('Link for Title'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['link']) ? $items[$delta]['link'] : NULL,
      );

      $element['fid'] = array(
      '#title' => t('File'),
      '#type' => 'managed_file',
      '#upload_location' => file_field_widget_uri($field, $instance),
      '#default_value' => isset($items[$delta]['fid']) ? $items[$delta]['fid'] : NULL,
        '#upload_validators' => array(
          'file_validate_extensions' => array('mp4 ogg jpg jpeg png gif'),
          'file_validate_size' => array(32*1024*1024),
        ),
      );

      $element['alt'] = array(
      '#title' => t('Image alt text'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['alt']) ? $items[$delta]['alt'] : NULL,
      );

      $element['description'] = array(
      '#title' => t('Text'),
      '#type' => 'textarea',
      '#rows' => '2',
      '#resizable' => FALSE,
      '#default_value' => isset($items[$delta]['description']) ? $items[$delta]['description'] : NULL,
      );

      $element['actionlink'] = array(
      '#title' => t('Button Link'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['actionlink']) ? $items[$delta]['actionlink'] : NULL,
      );


      $element['actiontitle'] = array(
      '#title' => t('Button Title'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['actiontitle']) ? $items[$delta]['actiontitle'] : NULL,
      );

      $element['actionlink2'] = array(
      '#title' => t('Button Link 2'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['actionlink2']) ? $items[$delta]['actionlink2'] : NULL,
      );

      $element['actiontitle2'] = array(
      '#title' => t('Button Title 2'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['actiontitle2']) ? $items[$delta]['actiontitle2'] : NULL,
      );

      $element['color'] = array(
      '#title' => t('Button color'),
      '#type' => 'select',
      '#options' => array('blue' => 'blue', 'red' => 'red'),
      '#default_value' => isset($items[$delta]['color']) ? $items[$delta]['color'] : 'blue',
      );

      $element['position'] = array(
      '#title' => t('Text position on image'),
      '#type' => 'select',
      '#options' => array('top-left' => 'top-left', 'top-center' => 'top-center', 'top-right' => 'top-right',
      'mid-left' => 'mid-left', 'mid-center' => 'mid-center', 'mid-right' => 'mid-right', 'bottom-left' => 'bottom-left',
      'bottom-center' => 'bottom-center', 'bottom-right' => 'bottom-right'),
      '#default_value' => isset($items[$delta]['position']) ? $items[$delta]['position'] : 'top-left',
      );

      $element['field_settings'] = array(
        '#title' => t('Field Settings'),
        '#type' => 'hidden',
        '#value' => '0',//this will definitely need to be changed
        '#prefix' => '<div id="field_settings_hidden">',
        '#suffix' => '</div>',
        '#default_value' => isset($encoded_loc) ? $encoded_loc : NULL,
      );


    }

    return $element;

  }

  /**
  * Implements hook_field_validate().
  */
  function webspark_superhero_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {


    foreach ($items as $delta => $item) {
      if (!webspark_superhero_field_is_empty($item, $field) && empty($item['fid'])) {

        $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'hero_image_required',
        'message' => t('!name field is required.', array('!name' => t('Image'))),
        );
      }


    }
  }

  /**
  * Implements hook_field_presave()
  */
  function webspark_superhero_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
    // We need to set presave values if fids are not set. This shouldn't effect save operations
    // form API validation will prevent this from ever happening unless previewing.
    //dpm($items, 'items in presave');
    if (!empty($items)) {

      foreach ($items as &$item) {
        if (empty($item['fid'])) {
          $fid = variable_get('panopoly_widgets_default_' . $entity->bundle . '_fid', NULL);

          if (!empty($fid)) {
            $item['fid'] = $fid;
          }
        }
      }
      file_field_presave($entity_type, $entity, $field, $instance, $langcode, $items);
    }
  }

  /**
  * Implements hook_field_load().
  */
  function webspark_superhero_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
    $entity_items_copy = $items;
    file_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
    // image_field_load() will overwrite 'title' with the image title. Here we
    // return it to the item title.
    foreach ($entity_items_copy as $entity_id => $items_copy) {
      foreach ($items_copy as $delta => $item) {
        $items[$entity_id][$delta]['title'] = $item['title'];
      }
    }
  }

  /**
  * Implements hook_field_insert().
  */
  function webspark_superhero_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
    file_field_insert($entity_type, $entity, $field, $instance, $langcode, $items);


     variable_set('panopoly_magic_live_preview', 1);

  }

  /**
  * Implements hook_field_update().
  */
  function webspark_superhero_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {

    // Load the original entity, if $entity->original is not already set.
    // This is done as per http://drupal.org/node/985642 and can be removed when that patch is committed.
    if (empty($entity->original)) {
      $entity->original = entity_load_unchanged($entity_type, $field['id']);
    }

    file_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
    variable_set('panopoly_magic_live_preview', 1);
  }

  /**
  * Implements hook_field_delete().
  **/
  function webspark_superhero_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
    file_field_delete($entity_type, $entity, $field, $instance, $langcode, $items);
  }

  /**
  * Implements hook_field_delete_revision().
  **/
  function webspark_superhero_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
    file_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, $items);
  }

  /**
  * Implements hook_field_prepare_view().
  **/
  function webspark_superhero_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
    // If there are no files specified at all, use the default.
    // TODO - make sure this works
    foreach ($entities as $id => $entity) {
      if (empty($items[$id]) && $field['settings']['default_image']) {
        if ($file = file_load($field['settings']['default_image'])) {
          $items[$id][0] = (array) $file + array(
          'is_default' => TRUE,
          'alt' => '',
          'title' => '',
          );
        }
      }
    }
  }
