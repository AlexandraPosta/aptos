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
          background: #232429;
          border-radius: 5px;
          box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
          color: white;
          padding: ${padding};
        }
        .card-header {
          font-size: 1em;
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