const blink = () => {
    let welcome = document.querySelector('#welcome');

    if (welcome.style.visibility === 'hidden')
    {
        welcome.style.visibility = 'visible';
    }
    else
    {
        welcome.style.visibility = 'hidden';
    }
}

window.setInterval(blink, 250);
