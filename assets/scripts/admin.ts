(function () {
  'use strict';

  document.addEventListener('DOMContentLoaded', navBarMobile);

  function navBarMobile() {
    const navbarBurgers: Array<any> = Array.prototype.slice.call(document.querySelectorAll('.navbar-burger'), 0);
    if (navbarBurgers.length > 0) {
      navbarBurgers.forEach((el: Element) => {
        el.addEventListener('click', navbarMobileClick);
      });
    }
  }

  function navbarMobileClick(e: Event) {
    const element = e.currentTarget;
    const elementId = (<HTMLElement>element).getAttribute('data-target');
    const target = document.querySelector(`#${elementId}`);

    (<HTMLElement>element).classList.toggle('is-active');
    if (target) {
      target.classList.toggle('is-active');
    }
  }

  // TODO: make table row selected (.is-selected) when table is generated from VBO
})();