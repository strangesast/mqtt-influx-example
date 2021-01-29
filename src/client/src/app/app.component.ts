import { Component } from "@angular/core";

@Component({
  selector: "app-root",
  template: `
    <div>App!</div>
    <button mat-stroked-button>Button</button>
    <router-outlet></router-outlet>
  `,
  styles: [],
})
export class AppComponent {}
