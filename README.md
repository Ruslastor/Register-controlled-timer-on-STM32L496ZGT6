<h1>Register controlled timer with STM32L496ZGT6 microcontroller (Chameleon board)</h1>

<p>The aim of this small project was to make a timer, that counts from 0 to 9999, with the use of 4 digit 7-segment display, and not using HAL standard library, but configure GPIO and TIM6 timer by hand, using registers.</p>
<p>The stucture of display is as follows:</p>
<img src="images_7s/segment.png"/>
<p>From also this circuitry document, we can also see, that segments are connected like:</p>
<table>
  <tr>
    <th>Seg_A</th>
    <th>Seg_B</th>
    <th>Seg_C</th>
    <th>Seg_D</th>
    <th>Seg_E</th>
    <th>Seg_F</th>
    <th>Seg_G</th>
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
    <th>Dig_1</th>
    <th>Dig_2</th>
    <th>Dig_3</th>
    <th>Dig_4</th>
  </tr>
  <tr>
    <td>PB2</td>
    <td>PB3</td>
    <td>PB4</td>
    <td>PB5</td>
  </tr>
</table>


https://kameleonboard.org/wp-content/uploads/STM32L496ZGT6-Reference-Manual.pdf
