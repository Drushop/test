<?php

/**
 * @file
 * Display Suite 3 column equal width template.
 */
?>
<<?php print $layout_wrapper; print $layout_attributes; ?> class="ds-3col-equal <?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="container">
  <div class="row">
  <<?php print $left_wrapper ?> class="col-4 col-sm-12<?php print $left_classes; ?>">
    <?php print $left; ?>
  </<?php print $left_wrapper ?>>
  <<?php print $middle_wrapper ?> class="col-6 col-sm-12<?php print $middle_classes; ?>">
    <?php print $middle; ?>
  </<?php print $middle_wrapper ?>>
  </div>
  </div>
  <<?php print $right_wrapper ?> class="<?php print $right_classes; ?> sdf">
    <?php print $right; ?>
  </<?php print $right_wrapper ?>>
</<?php print $layout_wrapper ?>>


<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
