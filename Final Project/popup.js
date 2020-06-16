let tabsHolder = document.getElementById('tabsContainer');
let allTabs;
const googleIcon = 'https://www.google.com/images/icons/product/chrome-32.png';

chrome.tabs.query({ currentWindow: true }, tabs => {
  allTabs = tabs;

  allTabs.map(tab => {
    const newDiv = document.createElement('div');
    newDiv.classList.add('card', 'tabCard');
    newDiv.id = `tab${tab.id}`;
    newDiv.innerHTML += `
        <div class="card-body tabCardBody">
            ${
              tab.favIconUrl
                ? `<span class="imgSpan">
                      <img style="width: 24px; height: 24px" src=${tab.favIconUrl} />
                    </span>
                  `
                : `<span class="imgSpan">
                      <img style="width: 24px; height: 24px" src=${googleIcon} />
                    </span>
                  `
            }
            <div class="tabCardTitle">
              ${tab.title}
            </div>
        </div>
      `;
    newDiv.addEventListener('click', tabSwitch, false);

    tabsHolder.appendChild(newDiv);
  });
});

const tabSwitch = e => {
  const tabId =
    e.path.length == 7 ? parseInt(e.path[1].id.substring(3)) : parseInt(e.path[2].id.substring(3));

  chrome.tabs.update(tabId, { active: true });

  document.getElementById(`tab${tabId}`).classList.add = highlightStyle[2];
};
