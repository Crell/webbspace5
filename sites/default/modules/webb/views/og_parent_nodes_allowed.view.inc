<?php

  $view = new stdClass();
  $view->name = 'og_parent_nodes_allowed';
  $view->description = 'List notes that can be replied to on a given OG.';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = FALSE;
  $view->page_title = '';
  $view->page_header = '';
  $view->page_header_format = '6';
  $view->page_footer = '';
  $view->page_footer_format = '6';
  $view->page_empty = '';
  $view->page_empty_format = '6';
  $view->page_type = 'node';
  $view->url = '';
  $view->use_pager = FALSE;
  $view->nodes_per_page = '0';
  $view->sort = array (
  );
  $view->argument = array (
  );
  $view->field = array (
  );
  $view->filter = array (
    array (
      'tablename' => 'og_ancestry',
      'field' => 'picg',
      'operator' => '=',
      'options' => '',
      'value' => '***CURRENT_GID***',
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'note',
),
    ),
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'created',
      'operator' => '>=',
      'options' => '-7776000',
      'value' => 'now',
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(og_ancestry, node);
  $views[$view->name] = $view;
