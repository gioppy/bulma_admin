"use strict";
(function () {
    'use strict';
    document.addEventListener('DOMContentLoaded', navBarMobile);
    function navBarMobile() {
        var navbarBurgers = Array.prototype.slice.call(document.querySelectorAll('.navbar-burger'), 0);
        if (navbarBurgers.length > 0) {
            navbarBurgers.forEach(function (el) {
                el.addEventListener('click', navbarMobileClick);
            });
        }
    }
    function navbarMobileClick(e) {
        var element = e.currentTarget;
        var elementId = element.getAttribute('data-target');
        var target = document.querySelector("#" + elementId);
        element.classList.toggle('is-active');
        if (target) {
            target.classList.toggle('is-active');
        }
    }
    // TODO: make table row selected (.is-selected) when table is generated from VBO
})();
