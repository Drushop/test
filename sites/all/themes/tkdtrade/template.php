<?php
  function tkdtrade_preprocess_dc_ajax_shopping_cart_teaser(&$variables) {
    $currency = dc_ajax_add_cart_currency_load();
    $image_cart_icon_path = drupal_get_path('theme', 'tkdtrade') . '/images/Cart.svg';
  
    $variables['total_amount'] = commerce_currency_format($variables['total']['amount'], $currency['code']);
    $variables['cart_icon'] = theme('image', array(
      'path' => $image_cart_icon_path,
    ));
  
    // Create new theme variables related to configuration.
    $variables['configuration']['empty_cart_teaser_message'] = variable_get(DC_AJAX_ADD_CART_EMPTY_CART_TEASER_MESSAGE, t('Empty'));
    $cart_link_text = format_plural($variables['quantity'], '1 ' . variable_get(DC_AJAX_ADD_CART_ITEM_SUFFIX_TEXT_SINGULAR, t('item')), '@count ' . variable_get(DC_AJAX_ADD_CART_ITEM_SUFFIX_TEXT_PLURAL, t('items')));
    $variables['cart_link'] = l($cart_link_text, 'cart', array('attributes' => array('class' => array('quantity'))));
  }