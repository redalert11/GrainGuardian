/* globals Chart:false */

(() => {
  'use strict'

  // Graphs
  const ctx = document.getElementById('myChart')
  // eslint-disable-next-line no-unused-vars
  const myChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: [  // Labels at the bottom of the graph
        'Sunday',
        'Monday',
        'Tuesday',
        'Wednesday',
        'Thursday',
        'Friday',
        'Saturday'
      ],
      datasets: [
        {
          label: '6 inches', // Edit this label to change the text at the top of the graph for each line
          data: [
            15339,
            21345,
            18483,
            24003,
            23489,
            24092,
            12034
          ],
          lineTension: 0,
          backgroundColor: 'transparent',
          borderColor: '#007bff',
          borderWidth: 4,
          pointBackgroundColor: '#007bff',
        },
        {
          label: '12 inches',
          data: [
            12345,
            23456,
            12345,
            23456,
            12345,
            23456,
            12345
          ],
          lineTension: 0,
          backgroundColor: 'transparent',
          borderColor: '#ff5733',
          borderWidth: 4,
          pointBackgroundColor: '#ff5733',
        }
      ]
    },
    options: {
      plugins: {
        legend: {
          display: true
        },
        tooltip: {
          boxPadding: 3
        }
      }
    }
  })
})()

document.addEventListener('DOMContentLoaded', function () {
  var systemSubmenuToggle = document.querySelector('a[href="#systemSubmenu"]');
  var chevronIcon = systemSubmenuToggle.querySelector('.toggle-icon');

  systemSubmenuToggle.addEventListener('click', function () {
    chevronIcon.classList.toggle('rotate');
  });

  var collapseElement = document.querySelector('#systemSubmenu');
  collapseElement.addEventListener('shown.bs.collapse', function () {
    chevronIcon.classList.add('rotate');
  });
  collapseElement.addEventListener('hidden.bs.collapse', function () {
    chevronIcon.classList.remove('rotate');
  });
});