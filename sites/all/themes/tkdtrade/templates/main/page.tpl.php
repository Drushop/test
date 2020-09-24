<div class="page">
    <div class="main-content">
        
        <header class="header">
            <div class="header-row">
            <div class="contacts">
                    <?php $block = module_invoke('block', 'block_view', '1'); print render($block['content']); ?>
                </div>
                <div class="logo">
                    <a href="<?php print $front_page ?>">
                        <img src='<?php print base_path().path_to_theme()."/images/logo.svg"; ?> ' alt="" />
                    </a>
                </div>
                <div class="search">
                    <div class="user-menu">
                        <?php $block = module_invoke('system', 'block_view','user-menu'); print render($block['content']); ?>
                        <?php $block = module_invoke('dc_ajax_add_cart', 'block_view','ajax_shopping_cart_teaser'); print render($block['content']); ?>
                    </div>
                    
                    <div class="search-block">
                        <?php $block = module_invoke('views', 'block_view', '-exp-product_catalog-page_1'); print render($block['content']); ?>
                    </div>
            </div>
            </div>
            <div class="mobile-header">
                <div class="mobile-button">
                    
                </div>
                <div class="mobile-logo">
                    <img src='<?php print base_path().path_to_theme()."/images/logo.svg"; ?> ' alt="" />
                </div>
                <div class="mobile-header-search">
                    
                </div>
                
            </div>
            
            <?php $block = module_invoke('superfish', 'block_view','1'); print render($block['content']); ?>
            
        </header>
        <div class="mobile-search-block">
            <?php $block = module_invoke('views', 'block_view', '-exp-product_catalog-page_1'); print render($block['content']); ?>
        </div>
        <?php print $breadcrumb; ?>
        <div class="content-block">
        <?php print $messages; ?>

            <?php print render($page['content']); ?>
        </div>
    </div>
    <footer class="footer">
        футер
    </footer>
</div>