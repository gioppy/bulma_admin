<nav class="navbar">
  <div class="navbar-brand">
    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="navbar-item">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>
    <div class="navbar-burger burger" data-target="navigation">
      <span></span>
      <span></span>
      <span></span>
    </div>
  </div>

  <?php /*if ($management_menu): */?><!--
    <div id="navigation" class="navbar-menu">
      <?php /*print $management_menu; */?>

      <div class="navbar-end">

      </div>
    </div>
  --><?php /*endif; */?>
</nav>

<div class="hero is-primary">
  <div class="hero-body">
    <div class="container">
      <h1 class="title is-2"><?php print $title; ?></h1>
      <?php if (!empty($page['help'])): ?>
      <div class="subtitle">
        <?php print render($page['help']); ?>
      </div>
      <?php endif; ?>
    </div>
  </div>

  <!--<div class="hero-foot">
    <div class="container">
      <?php /*if ($tabs && !empty($tabs['#primary'])): */?><div class="tabs is-boxed is-right"><ul><?php /*print render($tabs['#primary']); */?></ul></div><?php /*endif; */?>
    </div>
  </div>-->
</div>

<div class="hero is-info">
  <div class="hero-content">
    <div class="container">
      <div class="level ba-level--actions">
        <div class="level-left">
          <?php if (!empty($action_links)): ?>
            <ul class="action-links field is-grouped"><?php print render($action_links); ?></ul>
          <?php endif; ?>
        </div>
        <div class="level-right">
          <?php if ($tabs && !empty($tabs['#primary'])): ?><div class="tabs is-toggle is-right"><ul><?php print render($tabs['#primary']); ?></ul></div><?php endif; ?>
        </div>
      </div>
    </div>
  </div>
</div>

<div class="navbar has-shadow">
  <div class="container">
    <div class="navbar-start">
      <?php if ($breadcrumb): ?>
      <?php print $breadcrumb; ?>
      <?php endif; ?>
    </div>
    <?php if ($tabs && !empty($tabs['#secondary'])): ?>
      <div class="navbar-end">
        <div class="tabs is-toggle is-right is-small"><ul><?php print render($tabs['#secondary']); ?></ul></div>
      </div>
    <?php endif; ?>
  </div>
</div>

<section class="section">
  <div class="container">
    <?php print $messages; ?>
    <?php print render($page['content']); ?>
  </div>
</section>
