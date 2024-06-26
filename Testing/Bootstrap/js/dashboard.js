/* globals Chart:false */

(() => {
  'use strict'

  // Graphs
  const ctx = document.getElementById('myChart')
  // eslint-disable-next-line no-unused-vars
  const myChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: [
        'Sunday',
        'Monday',
        'Tuesday',
        'Wednesday',
        'Thursday',
        'Friday',
        'Saturday'
      ],
      datasets: [{
        label: 'Dataset 1',
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
        pointBackgroundColor: '#007bff'
      }, {
        label: 'Dataset 2',
        data: [
          12345,
          19345,
          20483,
          22003,
          19489,
          20092,
          10034
        ],
        lineTension: 0,
        backgroundColor: 'transparent',
        borderColor: '#ff5733',
        borderWidth: 4,
        pointBackgroundColor: '#ff5733'
      }, {
        label: 'Dataset 3',
        data: [
          10339,
          15345,
          16483,
          19003,
          18489,
          20092,
          9034
        ],
        lineTension: 0,
        backgroundColor: 'transparent',
        borderColor: '#28a745',
        borderWidth: 4,
        pointBackgroundColor: '#28a745'
      }]
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
