

<?php
$node = node_load($nid);
$image = field_get_items('node', $node, 'field_product_photo');
$output = field_view_value('node', $node, 'field_product_photo', $image['uri'], array(
  'type' => 'image',
  'settings' => array(
    'image_style' => 'thumbnail',
    'image_link' => 'content',
  ),
));
print render($output); 
?>