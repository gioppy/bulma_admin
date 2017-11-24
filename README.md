# void
A blank, Drupal 7 empty template

This template for Drupal 7 is used only for starting a new one. It is based mainly for my necessity and best practices.

If you wanto to use, remember to overwrite all occurances of "void" name: the folder name, the .info file and the functions inside of template.php.

The hooks in the template.php is the most used in my projects. If one is not necessary in your project, please delete it!

The most important hook is *hook_css_alter*: with this hook, I remove all the unnecessary CSS provided by Drupal and by some contrib (most used) modules. This is a best option if you want to use your style, without continue rewriting the Drupal styles.