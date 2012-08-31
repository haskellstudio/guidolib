<?php
  $resizepagetomusc = $_POST["resizepagetomusic"];
  if ($resizepagetomusic == '')
  {
    $resizepagetomusic = 'false';
  }
  $content = 'data={ ' 
. '"gmn" : "' . $_POST["gmn"] . '", '
. '"width" : "' . $_POST["width"] . '", '
. '"height" : "' . $_POST["height"] . '", '
. '"page" : "' . $_POST["page"] . '", '
. '"leftmargin" : "' . $_POST["marginleft"] . '", '
. '"rightmargin" : "' . $_POST["marginright"] . '", '
. '"topmargin" : "' . $_POST["margintop"] . '", '
. '"bottommargin" : "' . $_POST["marginbottom"] . '", '
. '"resizepagetomusic" : "' . $resizepagetomusic . '", '
. '"format" : "' . $_POST["format"] . '" }';

  $params = array(
    'http' => array( 
      'method' => 'POST', 
      'content' => $content
  )); 
  $ctx = stream_context_create($params); 
  $fp = @fopen('http://guido.grame.fr:8000/', 'rb', false, $ctx); 
  if (!$fp) { 
    throw new Exception("Couldn't connect to the server."); 
  } 
  $response = @stream_get_contents($fp);
  if ($response === false) { 
    throw new Exception("Didn't get a response from the server.");
  }
   
  $im = imagecreatefromstring($response);
  if ($im !== false) {
    header('Content-Type: image/' . $_POST["format"]);
    imagepng($im);
    imagedestroy($im);
  }
  else {
    throw new Exception("Didn't get an image from the server.");
  }
?>