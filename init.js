Object.defineProperty(window, "src", {
    get: function() {
        return window.location.href;
    },
    set: function(value) {
        window.location.href = value;
    }
});