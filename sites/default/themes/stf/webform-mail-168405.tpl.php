<?php /* GM  Report Form - Email*/ ?>
  Reporter's Name: <?php echo $form_values['submitted']['name'] ?>
  Ship: <?php echo $form_values['submitted']['ship'] ?> 
  Position: <?php echo $form_values['submitted']['position'] ?>
  
  Crew posting level: <?php echo $form_values['submitted']['crew_posting_level'] ?>
  Senior staff posting level: <?php echo $form_values['submitted']['senior_staff_posting_level'] ?>
  Overall ship health: <?php echo $form_values['submitted']['overall_ship_health'] ?>
  
  Sim overview: <?php echo $form_values['submitted']['sim_overview'] ?>
  
  Spotlight: <?php echo $form_values['submitted']['spotlights'] ?>
  
  *** DO NOT INCLUDE BELOW IN REPORT ***
  
  Personnel Department Recommendations:
  
  Submit: <? php echo $form_values['submitted']['submit'] ?>
  Person nominated: <?php echo $form_values['submitted']['person_nominated'] ?>
  Person nominated for: <?php echo $form_values['submitted']['person_nominated_for'] ?>
  
  Comments: <?php echo $form_values['submitted']['comments'] ?>
  
  Private Comments: <?php echo $form_values['submitted']['private_comments'] ?>
  
  <?php /* End CO/XO E-Mail */ ?>
  
  
  
	