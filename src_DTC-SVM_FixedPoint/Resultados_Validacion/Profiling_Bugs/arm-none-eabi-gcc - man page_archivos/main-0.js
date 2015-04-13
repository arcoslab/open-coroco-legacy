/*global YUI */
YUI({bootstrap: false}).use('*', function (Y) {
  'use strict';

  Y.one('body').addClass('yui3-skin-sam');

  function listenToExplainBox () {

    var
      commentTextInput = Y.one('#explain-comment'),
      shareTextInput = Y.one('#explain-link-to-share');

    Y.one('.explain-share').on('click', function (e) {
      var el = Y.one('.explain-share-dets');
      el.setStyle('display', el.getStyle('display') === 'block' ? 'none' : 'block');
      setShareLink();
    });
    shareTextInput.on('click', function (e) {
      e.currentTarget.select();
    });
    var setShareLink = function () {
      var
        url = 'https://www.mankier.com/?explain=' + encodeURIComponent(shareTextInput.getAttribute('data-cmd')),
        commentStr = commentTextInput.get('value');

      if (commentStr.length > 0) {
        url += '&comment=' + encodeURIComponent(commentStr);
      }

      shareTextInput.set('value', url + '#explain');
    };
    commentTextInput.on('valuechange', setShareLink);

  }

  Y.all('.in-search').each(function (searchInput) {

    var apiCall, selectFn;

    if (searchInput.hasClass('home-search')) {

      var explain = function (cmd) {
        Y.io('/api/explain/?format=html&q=' + encodeURIComponent(cmd), {
          on: {
            success: function (id, o) {
              Y.one('.explain-area').setHTML(o.responseText);
              if (Y.one('.explain-cmd-table')) { // we might get an error string instead of a table of explanations...
                listenToExplainBox();
              }
            }
          }
        });
      };

      searchInput.ancestor('form').on('submit', function (e) {
        e.preventDefault();
        explain(searchInput.get('value'));
      });
      apiCall = 'suggest';
      selectFn = function (e) {
        explain(e.result.text);
      };

      // Cheeky other bits and pieces now we know we're on the home page.
      if (Y.one('.explain-cmd-table')) {
        listenToExplainBox();
      }


    } else {
      apiCall = 'mans';
      selectFn = function (e) {
        document.location.href = e.result.raw.url;
      };
    }

    searchInput.plug(Y.Plugin.AutoComplete, {
      minQueryLength: 1,
      resultHighlighter: 'phraseMatch',
      resultListLocator: 'results',
      resultTextLocator: 'text',
      source: '/api/' + apiCall + '/?q={query}&callback={callback}',
      on: {
        select: selectFn
      }
    });
  });

  var menu = Y.one('#menu');
  function listenForOneOffMenuCloser () {
    Y.one('.man-page').once('click', function (e) {
      menu.removeClass('active');
    });
  }

  if (menu) {
  

    // This little convoluted bit is to stop Google page speed thinking we have tap targets too close together on load:
    if (menu.hasClass('active')) {
      listenForOneOffMenuCloser();
    }

    var lastSection = Y.one('div.section:last-of-type');
    if (lastSection) {
      var newPadding = parseInt(Y.one('body').get('winHeight'), 10) - parseInt(lastSection.getComputedStyle('height'), 10);
      if (newPadding < 0) {
        newPadding = 0;
      }
      Y.one('.man-page').setStyle('paddingBottom', newPadding + 'px');
    }

    var lis = menu.all('li');
    var scrollTimeout = false;

    var scrollAction = function () {
      var
        highlight,
        elNearTop = Y.one(document.elementFromPoint(320, 100));


      if (elNearTop) {
        if (elNearTop.hasClass('subsection') || elNearTop.hasClass('section')) {
          highlight = elNearTop;
        } else {
          highlight = elNearTop.ancestor('div.subsection');
          if (!highlight) {
            highlight = elNearTop.ancestor('div.section');
          }
        }
        if (highlight) {
          var sectionId = highlight.get('id');
          clearMenuEntries();
          var menuItem = menu.one('a[href="#' + sectionId + '"]');
          if (menuItem) {
            var li = menuItem.ancestor('li');
            if (li) {
              li.addClass('selected');
              var topPos = li.get('offsetTop');
              if (li.ancestor('.nav-h3')) {
                topPos = li.ancestor('li').addClass('selected').get('offsetTop');
              }
            }
            menu.set('scrollTop', topPos - 20);
          }
        } else {
          clearMenuEntries();
          menu.set('scrollTop', 0);
        }
      } else {
        clearMenuEntries();
      }

    };

    var clearMenuEntries = function () {
      lis.removeClass('selected');
    };

    Y.on('scroll', function (e) {
      clearTimeout(scrollTimeout);
      scrollTimeout = setTimeout(scrollAction, 100);
    });

    lis.on('click', function (e) {
      menu.removeClass('active');
      clearMenuEntries();
      e.currentTarget.addClass('selected');
    });

    var burger = Y.one('.burger');
    if (burger) {
      burger.on('click', function (e) {
        if (menu.hasClass('active')) {
          menu.removeClass('active');
        } else {
          menu.addClass('active');
          listenForOneOffMenuCloser();
        }
      });
    }
  }


});window.onhashchange = function () {
  var dt = document.getElementById(window.location.hash.substr(1)).parentNode;
  while (dt) {
    if (dt.nodeName === 'DT') {
      break;
    }
    dt = dt.parentNode;    
  }

  if (!dt) {
    return;
  }

  var testNode = dt.previousSibling, targetNodes = [dt];

  while (testNode) {
    if (testNode.nodeType === 1) {
      if (testNode.nodeName !== 'DT') {
        break;
      }
      targetNodes.push(testNode);
    }
    testNode = testNode.previousSibling;
  }
  testNode = dt.nextSibling;
  while (testNode) {
    if (testNode.nodeType === 1) {
      if (testNode.nodeName !== 'DT' && testNode.nodeName !== 'DD') {
        break;
      }
      targetNodes.push(testNode);
      if (testNode.nodeName === 'DD') {
        break; // only expect on of these...
      }
    }
    testNode = testNode.nextSibling;
  }

  targetNodes.forEach(function (n) {
    n.classList.add('custom-target');
  });

  setTimeout (function () {
    targetNodes.forEach(function (n) {
      n.classList.remove('custom-target');
    });
  }, 2000);

};