<h1>Register controlled timer with STM32L496ZGT6 microcontroller (Kameleon board)</h1>

<p>The aim of this small project was to make a timer, that counts from 0 to 9999, with the use of 4 digit 7-segment display, and not using HAL standard library, but configure GPIO and TIM6 timer by hand, using registers.</p>
<p>The stucture of display is as follows:</p>
<img src="images_7s/segment.png"/>
<p>From also this circuitry document, we can also see, that segments are connected like:</p>
<table>
  <tr>
    <th>7S_A</th>
    <th>7S_B</th>
    <th>7S_C</th>
    <th>7S_D</th>
    <th>7S_E</th>
    <th>7S_F</th>
    <th>7S_G</th>
  </tr>
  <tr>
    <td>PG0</td>
    <td>PG1</td>
    <td>PG2</td>
    <td>PG3</td>
    <td>PG4</td>
    <td>PG5</td>
    <td>PG6</td>
  </tr>
</table>

<p>Cathode connections:</p>
<table>
  <tr>
    <th>7S_DIG1</th>
    <th>7S_DIG2</th>
    <th>7S_DIG3</th>
    <th>7S_DIG4</th>
  </tr>
  <tr>
    <td>PB2</td>
    <td>PB3</td>
    <td>PB4</td>
    <td>PB5</td>
  </tr>
</table>

<p>Button input:</p>
<table>
  <tr>
    <th>JOY_OK</th>
  </tr>
  <tr>
    <td>PE15</td>
  </tr>
</table>

https://kameleonboard.org/wp-content/uploads/STM32L496ZGT6-Reference-Manual.pdf
