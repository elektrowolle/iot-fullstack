<template>
  <div id="app">
    <h1>State of drawers</h1>
    <ul>
      <li v-for="drawer, key in drawers">
        <input :drawer="key" name="name" @input="updateDrawer" :value="drawer.name">
        <input :drawer="key" name="eui" :value="drawer.eui" disabled>
        <input :drawer="key" name="occupied" @input="updateDrawer" :value="drawer.occupied" type="checkbox">
      </li>
    </ul>
    <button @click="addDrawer()">Add</button>
  </div>
</template>

<script>
import Store from "./Store.js"
import {mapState} from "vuex"

export default {
  name: 'app',
  store: Store,

  data () {
    return {}
  },

  computed:
    mapState({
      drawers: 'drawers',
    }),

  methods:{
    addDrawer(){
      this.$store.commit('addDrawer', {
        name: "",
        eui: Math.floor(Math.random() * 10000),
        occupied: false,
      })
    },

    updateDrawer(e){
      let drawer = e.target.attributes.drawer;
      let value = e.target.value;
      this.$store.commit('updateDrawer', {
        [e.target.name]: value,
        id             : drawer.value
      });
    }
  }

}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  /*display: inline-block;*/
  margin: 0 10px;
}

a {
  color: #42b983;
}
</style>
