/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview Custon widgets class.
 * @author A. Posta
*/

/**
* CustomCard is a custom element that renders a card with a 
* title and content. 
* @class CustomCard 
* @extends HTMLElement
*/
class CustomCard extends HTMLElement {
  constructor() {
    super(); 
    this.attachShadow({ mode: 'open' }); // Attach a shadow root to the element.
    this.addEventListener('click', e => { 
      // get png of current state of the card
    });
    this.render();
  }

  connectedCallback() {
    this.render();
  }

  static get observedAttributes() {
    return ['title', 'width', 'height', 'content'];
  }

  attributeChangedCallback() {
    this.render();
  }

  /**
   * Render the card with the current attributes.
   * @returns {void} None
  */
  render() {
    const title = this.getAttribute('title') || 'Default Title';
    const width = this.getAttribute('width') || 'auto';
    const height = this.getAttribute('height') || 'auto';
    const padding = this.getAttribute('padding') || '15px';

    this.shadowRoot.innerHTML = `
      <style>
        :host {
          min-width: ${width};
          min-height: ${height};
        }
        .card {
          background: white;
          border-radius: 5px;
          box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
          padding: ${padding};
        }
        .card-header {
          font-size: 1em;
          font-weight: bold;
          margin-bottom: 10px;
        }
        .card-content {
          font-size: 0.8em;
        }
      </style>
      <div class="card">
        <div class="card-header">${title}</div>
        <div class="card-content">
          <slot></slot> <!-- Slot for user-provided content -->
        </div>
      </div>
    `;
  }
}

// Define the new element.
customElements.define('custom-card', CustomCard);