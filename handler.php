
<?php
 
$message = "Имя: {$_POST['name']}<br>"; // добавляем имя в текст
$message .= "Телефон: {$_POST['phone']}"; // добавляем телефон в текст
 
require 'phpmailer/PHPMailerAutoload.php';
 
$mail = new PHPMailer;
$mail->isSMTP();
 $email = $_POST['email'];
$mail->Host = 'smtp.mail.ru';
$mail->SMTPAuth = true;
$mail->Username = 'appdevice@yandex.ru'; // логин от вашей почты
$mail->Password = 'eA#VjE9BNS'; // пароль от почтового ящика
$mail->SMTPSecure = 'ssl';
$mail->Port = '465';
$mail->CharSet = 'UTF-8';
$mail->From = 'mailer_test_2014@mail.ru';
$mail->FromName = 'Андрей';
$mail->addAddress($email, 'Андрей');
$mail->isHTML(true);
$mail->Subject = 'Заказ обратного звонка';
$mail->Body = $message;
 
if( $mail->send() ){
 echo '<p style="color: green;">Ваше сообщение отправлено</p>';
}else{
 echo '<p style="color: red;">Ошибка!</p>';
}
?>
