<h>Register controlled timer with STM32L496ZGT6 microcontroller (Chameleon board)</h>

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
    <tb>PG0</tb>
    <tb>PG1</tb>
    <tb>PG2</tb>
    <tb>PG3</tb>
    <tb>PG4</tb>
    <tb>PG5</tb>
    <tb>PG6</tb>
  </tr>
</table>


https://kameleonboard.org/wp-content/uploads/STM32L496ZGT6-Reference-Manual.pdf
