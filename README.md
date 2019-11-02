<h1 align="center">
  <br>
    <a href="logo.png">
      <img src="logo.png" alt="Houston, we have a problem" width="200">
    </a>
  <br>
</h1>

<h4 align="center"> Project 4 - Houston, we have a problem </h4>

<p align="center">
  <a href="#about-the-project">About the project</a> •
  <a href="#how-to-use">How To Use</a> •
</p>

<p align="center">
  <img src="timelapse.gif">
</p>


## About the project

This project was developed for the discipline
**Algorithm Project** of FGA College in the semester 02/2019.

This project implements a real-time flight panel that will
signal when two aircraft are dangerously close.

The main goal was to develop an application that uses
**divide and conquer** algorithm design paradigm.

We use data provided by the API
[opensky-network](https://opensky-network.org)
to get the coordinates of the aircraft that are located in
a certain region.

We set the map of the metropolitan region of São Paulo and
use the following coordinates for API requests.

Lower left: (-24.493696, -48.177301)

Superior right: (-22.797153, -45.566508)

These coordinates get the following region

<p align="center">
  <img src="spmap.png">
</p>

[EXPLICAÇÃO DO MOURA DE O QUE É FEITO COM OS DADOS RETORNADOS DA API]

The aircraft coordinate data is used in the Closest pair of
points problem algorithm
[1](https://en.wikipedia.org/wiki/Closest_pair_of_points_problem).

During the execution of the algorithm, the distance between
two aircraft is computed, if this distance is smaller than
the minimum configured distance, this pair of points is saved.

## How To Use

  ```bash
  # Clone this repository
  $ git clone https://github.com/projeto-de-algoritmos-2019-2/project-4-flight-panel flight-panel

  # Install virtualenv
  $ sudo pip3 install virtualenv

  # Create a env
  $ virtualenv -p python3 env

  # Activate the env
  $ source env/bin/activate

  # Install the requirements
  $ pip install -r requirements.txt

  # get in flight panel directory
  $ cd flight-panel

  # Run the application
  $ python panel.py
  ```
